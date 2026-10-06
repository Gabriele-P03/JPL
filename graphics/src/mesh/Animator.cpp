#include "Animator.hpp"

#include <algorithm>
#include <cmath>

#include <glm/gtc/matrix_transform.hpp>

namespace jpl {
    namespace _graphics {
        namespace _mesh {

            // ------------------------------------------------------------------
            // Sampling helpers (internal to this translation unit)
            // ------------------------------------------------------------------
            namespace {

                // glTF stores quaternions as (x, y, z, w); glm::quat wants (w, x, y, z)
                glm::quat toQuat(const glm::vec4& v) {
                    return glm::quat(v.w, v.x, v.y, v.z);
                }

                /**
                 * Samples a translation / rotation / scale track at the given time.
                 * Result is a vec4: (x,y,z,_) for T/S, (x,y,z,w) for rotation.
                 */
                glm::vec4 sampleTrack(const Sampler& sp, float time, bool isRotation) {
                    const bool cubic = sp.interp == Interp::Cubic;

                    // Value of keyframe k (cubic layout: in-tangent, value, out-tangent)
                    auto val = [&](std::size_t k) { return sp.values[cubic ? k * 3 + 1 : k]; };

                    const std::size_t n = sp.times.size();

                    // Clamp outside of the keyframe range
                    if (n == 1 || time <= sp.times.front()) return val(0);
                    if (time >= sp.times.back())            return val(n - 1);

                    // Find the keyframe interval [k0, k1] containing 'time'
                    std::size_t k1 = std::upper_bound(sp.times.begin(), sp.times.end(), time) - sp.times.begin();
                    std::size_t k0 = k1 - 1;
                    const float dt = sp.times[k1] - sp.times[k0];
                    const float u  = (time - sp.times[k0]) / dt;

                    switch (sp.interp) {
                        case Interp::Step:
                            return val(k0);

                        case Interp::Linear:
                            if (isRotation) {
                                glm::quat q = glm::slerp(toQuat(val(k0)), toQuat(val(k1)), u);
                                return glm::vec4(q.x, q.y, q.z, q.w);
                            }
                            return glm::mix(val(k0), val(k1), u);

                        case Interp::Cubic: {
                            // Cubic Hermite spline, tangents scaled by the interval length (glTF spec)
                            const glm::vec4 p0 = val(k0), p1 = val(k1);
                            const glm::vec4 m0 = sp.values[k0 * 3 + 2] * dt;   // out-tangent of k0
                            const glm::vec4 m1 = sp.values[k1 * 3]     * dt;   // in-tangent of k1
                            const float u2 = u * u, u3 = u2 * u;
                            glm::vec4 res = (2*u3 - 3*u2 + 1) * p0 + (u3 - 2*u2 + u) * m0
                                          + (-2*u3 + 3*u2)    * p1 + (u3 - u2)       * m1;
                            return isRotation ? glm::normalize(res) : res;
                        }
                    }
                    return val(k0);
                }

                /**
                 * Samples morph target weights (N values per keyframe) at the given time.
                 */
                void sampleWeights(const Sampler& sp, float time, std::vector<float>& out) {
                    const std::size_t N = sp.numTargets;
                    const bool cubic = sp.interp == Interp::Cubic;
                    const std::size_t stride = cubic ? 3 * N : N;   // floats per keyframe
                    const std::size_t off    = cubic ? N : 0;       // skip in-tangents

                    auto val = [&](std::size_t k, std::size_t i) { return sp.weights[k * stride + off + i]; };

                    out.resize(N);
                    const std::size_t n = sp.times.size();

                    if (n == 1 || time <= sp.times.front()) {
                        for (std::size_t i = 0; i < N; ++i) out[i] = val(0, i);
                        return;
                    }
                    if (time >= sp.times.back()) {
                        for (std::size_t i = 0; i < N; ++i) out[i] = val(n - 1, i);
                        return;
                    }

                    std::size_t k1 = std::upper_bound(sp.times.begin(), sp.times.end(), time) - sp.times.begin();
                    std::size_t k0 = k1 - 1;
                    const float dt = sp.times[k1] - sp.times[k0];
                    const float u  = (time - sp.times[k0]) / dt;
                    const float u2 = u * u, u3 = u2 * u;

                    for (std::size_t i = 0; i < N; ++i) {
                        switch (sp.interp) {
                            case Interp::Step:
                                out[i] = val(k0, i);
                                break;
                            case Interp::Linear:
                                out[i] = val(k0, i) + (val(k1, i) - val(k0, i)) * u;
                                break;
                            case Interp::Cubic: {
                                const float m0 = sp.weights[k0 * stride + 2 * N + i] * dt;  // out-tangent of k0
                                const float m1 = sp.weights[k1 * stride + i] * dt;          // in-tangent of k1
                                out[i] = (2*u3 - 3*u2 + 1) * val(k0, i) + (u3 - 2*u2 + u) * m0
                                       + (-2*u3 + 3*u2)    * val(k1, i) + (u3 - u2)       * m1;
                            } break;
                        }
                    }
                }
            }

            // ------------------------------------------------------------------
            // Animator
            // ------------------------------------------------------------------
            Animator::Animator(const Model& model)
                : model(model),
                  t(model.nodes.size()), s(model.nodes.size()), r(model.nodes.size()),
                  nodeWeights(model.nodes.size()),
                  global(model.nodes.size())
            {
                for (const auto& sk : model.skins)
                    this->joints.emplace_back(sk.joints.size(), glm::mat4(1.f));

                this->update(0.f);   // compute the rest pose immediately
            }

            bool Animator::play(const std::string& name, bool looping) {
                for (const auto& c : this->model.clips) {
                    if (c.name == name) {
                        this->clip = &c;
                        this->time = 0.f;
                        this->loop = looping;
                        return true;
                    }
                }
                return false;
            }

            void Animator::updateNode(int i, const glm::mat4& parent) {
                const glm::mat4 local = glm::translate(glm::mat4(1.f), this->t[i])
                                      * glm::mat4_cast(this->r[i])
                                      * glm::scale(glm::mat4(1.f), this->s[i]);
                this->global[i] = parent * local;

                for (int c : this->model.nodes[i].children)
                    this->updateNode(c, this->global[i]);
            }

            void Animator::update(float dt) {

                // 1. Reset to the rest pose (nodes not touched by the clip keep it)
                for (std::size_t i = 0; i < this->model.nodes.size(); ++i) {
                    this->t[i] = this->model.nodes[i].t;
                    this->r[i] = this->model.nodes[i].r;
                    this->s[i] = this->model.nodes[i].s;
                    this->nodeWeights[i] = this->model.nodes[i].weights;
                }

                // 2. Advance time and sample every channel
                if (this->clip) {
                    this->time += dt;
                    if (this->clip->duration > 0.f)
                        this->time = this->loop ? std::fmod(this->time, this->clip->duration)
                                                : std::min(this->time, this->clip->duration);

                    for (const auto& c : this->clip->channels) {
                        const Sampler& sp = this->clip->samplers[c.sampler];

                        if (c.path == Path::Weights) {
                            sampleWeights(sp, this->time, this->nodeWeights[c.node]);
                            continue;
                        }

                        const glm::vec4 v = sampleTrack(sp, this->time, c.path == Path::Rotation);
                        switch (c.path) {
                            case Path::Translation: this->t[c.node] = glm::vec3(v);                break;
                            case Path::Rotation:    this->r[c.node] = glm::normalize(toQuat(v));   break;
                            case Path::Scale:       this->s[c.node] = glm::vec3(v);                break;
                            default: break;
                        }
                    }
                }

                // 3. Global matrices (walk the hierarchy from the roots)
                for (int root : this->model.roots)
                    this->updateNode(root, glm::mat4(1.f));

                // 4. Joint matrices = globalJoint * inverseBind (what the shader needs)
                for (std::size_t k = 0; k < this->model.skins.size(); ++k) {
                    const Skin& sk = this->model.skins[k];
                    for (std::size_t j = 0; j < sk.joints.size(); ++j)
                        this->joints[k][j] = this->global[sk.joints[j]] * sk.inverseBind[j];
                }
            }
        }
    }
}

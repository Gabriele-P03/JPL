/**
 * glTF loader based on fastgltf (>= 0.8 API).
 *
 * Loads meshes (static or skinned), the node hierarchy, skins, animation clips
 * (translation / rotation / scale / morph weights) and morph targets.
 */

#define GLM_ENABLE_EXPERIMENTAL   // must come before any glm include (matrix_decompose)

#include "GltfLoader.hpp"

#include <algorithm>
#include <cstdint>
#include <memory>
#include <variant>

#include <fastgltf/core.hpp>
#include <fastgltf/tools.hpp>
#include <fastgltf/glm_element_traits.hpp>   // lets accessors be read directly as glm types

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include <jpl/exception/runtime/IllegalArgumentException.hpp>

namespace jpl {
    namespace _graphics {
        namespace _mesh {

            namespace {

                /**
                 * Builds one Mesh from one glTF primitive and fills its MorphSet.
                 * Returns nullptr if the primitive is unsupported (not triangles / no POSITION).
                 */
                Mesh* buildMesh(const fastgltf::Asset& asset,
                                const fastgltf::Primitive& prim,
                                MorphSet& morph)
                {
                    if (prim.type != fastgltf::PrimitiveType::Triangles) return nullptr;

                    auto posIt = prim.findAttribute("POSITION");
                    if (posIt == prim.attributes.end()) return nullptr;

                    auto uvIt = prim.findAttribute("TEXCOORD_0");
                    auto nIt  = prim.findAttribute("NORMAL");
                    auto jIt  = prim.findAttribute("JOINTS_0");
                    auto wIt  = prim.findAttribute("WEIGHTS_0");

                    // A primitive is skinned only if it has both joints and weights
                    const bool skinned = (jIt != prim.attributes.end()) && (wIt != prim.attributes.end());
                    const unsigned int stride = skinned ? FLOATS_PER_VERTEX_SKINNED
                                                        : FLOATS_PER_VERTEX_STATIC;

                    const auto& posAcc = asset.accessors[posIt->accessorIndex];
                    const std::size_t count = posAcc.count;

                    // Zero-initialised: missing UVs / normals simply stay at 0
                    std::unique_ptr<float[]> vertices = std::make_unique<float[]>(count * stride);

                    // ---- POSITION ----
                    fastgltf::iterateAccessorWithIndex<glm::vec3>(asset, posAcc,
                        [&](glm::vec3 p, std::size_t i) {
                            float* v = vertices.get() + i * stride;
                            v[0] = p.x; v[1] = p.y; v[2] = p.z;
                        });

                    // ---- TEXCOORD_0 ----
                    if (uvIt != prim.attributes.end()) {
                        fastgltf::iterateAccessorWithIndex<glm::vec2>(asset, asset.accessors[uvIt->accessorIndex],
                            [&](glm::vec2 uv, std::size_t i) {
                                float* v = vertices.get() + i * stride + OFFSET_UV;
                                v[0] = uv.x; v[1] = uv.y;
                            });
                    }

                    // ---- NORMAL ----
                    if (nIt != prim.attributes.end()) {
                        fastgltf::iterateAccessorWithIndex<glm::vec3>(asset, asset.accessors[nIt->accessorIndex],
                            [&](glm::vec3 n, std::size_t i) {
                                float* v = vertices.get() + i * stride + OFFSET_NORMAL;
                                v[0] = n.x; v[1] = n.y; v[2] = n.z;
                            });
                    }

                    // ---- JOINTS_0 / WEIGHTS_0 ----
                    if (skinned) {
                        fastgltf::iterateAccessorWithIndex<glm::uvec4>(asset, asset.accessors[jIt->accessorIndex],
                            [&](glm::uvec4 j, std::size_t i) {
                                float* v = vertices.get() + i * stride + OFFSET_JOINTS;
                                v[0] = float(j.x); v[1] = float(j.y); v[2] = float(j.z); v[3] = float(j.w);
                            });

                        // Normalised ubyte / ushort weights are converted to float by fastgltf
                        fastgltf::iterateAccessorWithIndex<glm::vec4>(asset, asset.accessors[wIt->accessorIndex],
                            [&](glm::vec4 w, std::size_t i) {
                                float* v = vertices.get() + i * stride + OFFSET_WEIGHTS;
                                v[0] = w.x; v[1] = w.y; v[2] = w.z; v[3] = w.w;
                            });

                        // Some exporters produce weights that do not sum to 1: renormalise
                        for (std::size_t i = 0; i < count; ++i) {
                            float* w = vertices.get() + i * stride + OFFSET_WEIGHTS;
                            const float sum = w[0] + w[1] + w[2] + w[3];
                            if (sum > 0.f && std::abs(sum - 1.f) > 1e-4f) {
                                for (int k = 0; k < 4; ++k) w[k] /= sum;
                            }
                        }
                    }

                    // ---- Morph targets: deltas relative to the base mesh ----
                    morph.targets.resize(prim.targets.size());
                    for (std::size_t t = 0; t < prim.targets.size(); ++t) {
                        MorphTarget& mt = morph.targets[t];
                        for (auto& attr : prim.targets[t]) {
                            // Sparse accessors and accessors without bufferView (all zeros)
                            // are handled by iterateAccessorWithIndex
                            if (attr.name == "POSITION") {
                                mt.dPos.assign(count, glm::vec3(0.f));
                                fastgltf::iterateAccessorWithIndex<glm::vec3>(asset, asset.accessors[attr.accessorIndex],
                                    [&](glm::vec3 d, std::size_t i) { mt.dPos[i] = d; });
                            } else if (attr.name == "NORMAL") {
                                mt.dNormal.assign(count, glm::vec3(0.f));
                                fastgltf::iterateAccessorWithIndex<glm::vec3>(asset, asset.accessors[attr.accessorIndex],
                                    [&](glm::vec3 d, std::size_t i) { mt.dNormal[i] = d; });
                            }
                        }
                    }

                    // ---- Indices (always converted to uint32) ----
                    std::unique_ptr<unsigned int[]> indices;
                    std::size_t indexCount = 0;

                    if (prim.indicesAccessor.has_value()) {
                        const auto& idxAcc = asset.accessors[*prim.indicesAccessor];
                        indexCount = idxAcc.count;
                        indices = std::make_unique<unsigned int[]>(indexCount);
                        // uint8 / uint16 / uint32 sources are all converted to uint32
                        fastgltf::copyFromAccessor<std::uint32_t>(asset, idxAcc, indices.get());
                    } else {
                        // Non-indexed geometry: generate 0..N-1
                        indexCount = count;
                        indices = std::make_unique<unsigned int[]>(indexCount);
                        for (std::size_t i = 0; i < indexCount; ++i) indices[i] = static_cast<unsigned int>(i);
                    }

                    // Ownership is transferred to the Mesh (its destructor must delete[] both arrays)
                    return new Mesh(vertices.release(),
                                    static_cast<unsigned int>(count * stride),
                                    OFFSET_UV,
                                    indices.release(),
                                    static_cast<unsigned int>(indexCount),
                                    stride,
                                    false);
                }

                Interp convertInterp(fastgltf::AnimationInterpolation i) {
                    switch (i) {
                        case fastgltf::AnimationInterpolation::Step:        return Interp::Step;
                        case fastgltf::AnimationInterpolation::CubicSpline: return Interp::Cubic;
                        default:                                            return Interp::Linear;
                    }
                }
            }

            Model loadGltfModel(const std::filesystem::path& path) {

                // ---------------------------------------------------------------
                // 1. Open and parse the file
                // ---------------------------------------------------------------
                auto data = fastgltf::GltfDataBuffer::FromPath(path);
                if (data.error() != fastgltf::Error::None) {
                    throw jpl::_exception::IllegalArgumentException("Unable to open the glTF file");
                }

                fastgltf::Parser parser;
                constexpr auto options = fastgltf::Options::LoadExternalBuffers
                                       | fastgltf::Options::LoadExternalImages;

                auto asset = parser.loadGltf(data.get(), path.parent_path(), options);
                if (asset.error() != fastgltf::Error::None) {
                    throw jpl::_exception::IllegalArgumentException("Invalid glTF file");
                }

                Model model;

                // ---------------------------------------------------------------
                // 2. Meshes: one Mesh* (and one MorphSet) per triangle primitive.
                //    meshPrims[m] remembers which Mesh* belong to glTF mesh m.
                // ---------------------------------------------------------------
                std::vector<std::vector<int>> meshPrims(asset->meshes.size());

                for (std::size_t m = 0; m < asset->meshes.size(); ++m) {
                    for (const auto& prim : asset->meshes[m].primitives) {
                        MorphSet morph;
                        if (Mesh* mesh = buildMesh(asset.get(), prim, morph)) {
                            meshPrims[m].push_back(static_cast<int>(model.meshes.size()));
                            model.meshes.push_back(mesh);
                            model.morphs.push_back(std::move(morph));   // keeps meshes/morphs parallel
                        }
                    }
                }

                // ---------------------------------------------------------------
                // 3. Nodes
                // ---------------------------------------------------------------
                model.nodes.resize(asset->nodes.size());

                for (std::size_t i = 0; i < asset->nodes.size(); ++i) {
                    const auto& n = asset->nodes[i];
                    Node& out = model.nodes[i];
                    out.name = std::string(n.name);

                    // A node transform is either TRS or a 4x4 matrix
                    if (const auto* trs = std::get_if<fastgltf::TRS>(&n.transform)) {
                        out.t = glm::vec3(trs->translation[0], trs->translation[1], trs->translation[2]);
                        // fastgltf quaternion order is (x, y, z, w); glm::quat constructor is (w, x, y, z)
                        out.r = glm::quat(trs->rotation[3], trs->rotation[0], trs->rotation[1], trs->rotation[2]);
                        out.s = glm::vec3(trs->scale[0], trs->scale[1], trs->scale[2]);
                    } else {
                        const auto& m = std::get<fastgltf::math::fmat4x4>(n.transform);
                        glm::vec3 skew; glm::vec4 persp;
                        glm::decompose(glm::make_mat4(m.data()), out.s, out.r, out.t, skew, persp);
                    }

                    for (auto c : n.children) {
                        out.children.push_back(static_cast<int>(c));
                        model.nodes[c].parent = static_cast<int>(i);
                    }

                    if (n.meshIndex.has_value()) {
                        const std::size_t mi = *n.meshIndex;
                        out.meshes = meshPrims[mi];

                        // Default morph weights: node weights override mesh weights
                        if (!n.weights.empty())
                            out.weights.assign(n.weights.begin(), n.weights.end());
                        else
                            out.weights.assign(asset->meshes[mi].weights.begin(),
                                               asset->meshes[mi].weights.end());
                    }

                    if (n.skinIndex.has_value()) out.skin = static_cast<int>(*n.skinIndex);
                }

                for (std::size_t i = 0; i < model.nodes.size(); ++i)
                    if (model.nodes[i].parent < 0) model.roots.push_back(static_cast<int>(i));

                // ---------------------------------------------------------------
                // 4. Skins
                // ---------------------------------------------------------------
                for (const auto& sk : asset->skins) {
                    Skin out;
                    for (auto j : sk.joints) out.joints.push_back(static_cast<int>(j));

                    // If the accessor is missing the spec says inverse bind matrices are identity
                    out.inverseBind.assign(sk.joints.size(), glm::mat4(1.f));
                    if (sk.inverseBindMatrices.has_value()) {
                        fastgltf::copyFromAccessor<glm::mat4>(asset.get(),
                            asset->accessors[*sk.inverseBindMatrices], out.inverseBind.data());
                    }
                    model.skins.push_back(std::move(out));
                }

                // ---------------------------------------------------------------
                // 5. Animations
                // ---------------------------------------------------------------
                for (const auto& a : asset->animations) {
                    Clip clip;
                    clip.name = std::string(a.name);

                    for (const auto& s : a.samplers) {
                        Sampler out;
                        out.interp = convertInterp(s.interpolation);

                        // Keyframe times
                        const auto& in = asset->accessors[s.inputAccessor];
                        out.times.resize(in.count);
                        fastgltf::copyFromAccessor<float>(asset.get(), in, out.times.data());
                        if (!out.times.empty())
                            clip.duration = std::max(clip.duration, out.times.back());

                        // Keyframe values
                        const auto& o = asset->accessors[s.outputAccessor];

                        if (o.type == fastgltf::AccessorType::Scalar) {
                            // Morph target weights: N values per keyframe (3*N for cubic spline)
                            out.weights.resize(o.count);
                            fastgltf::copyFromAccessor<float>(asset.get(), o, out.weights.data());
                            const std::size_t perKey = (out.interp == Interp::Cubic) ? 3 : 1;
                            out.numTargets = in.count ? o.count / (in.count * perKey) : 0;
                        } else if (o.type == fastgltf::AccessorType::Vec3) {
                            // Translation / scale
                            out.values.resize(o.count);
                            fastgltf::iterateAccessorWithIndex<glm::vec3>(asset.get(), o,
                                [&](glm::vec3 v, std::size_t i) { out.values[i] = glm::vec4(v, 0.f); });
                        } else if (o.type == fastgltf::AccessorType::Vec4) {
                            // Rotation (x, y, z, w)
                            out.values.resize(o.count);
                            fastgltf::iterateAccessorWithIndex<glm::vec4>(asset.get(), o,
                                [&](glm::vec4 v, std::size_t i) { out.values[i] = v; });
                        }

                        clip.samplers.push_back(std::move(out));
                    }

                    for (const auto& c : a.channels) {
                        if (!c.nodeIndex.has_value()) continue;

                        Path p;
                        switch (c.path) {
                            case fastgltf::AnimationPath::Translation: p = Path::Translation; break;
                            case fastgltf::AnimationPath::Rotation:    p = Path::Rotation;    break;
                            case fastgltf::AnimationPath::Scale:       p = Path::Scale;       break;
                            case fastgltf::AnimationPath::Weights:     p = Path::Weights;     break;
                            default: continue;
                        }
                        clip.channels.push_back({ static_cast<int>(*c.nodeIndex), p, static_cast<int>(c.samplerIndex) });
                    }

                    model.clips.push_back(std::move(clip));
                }

                return model;
            }
        }
    }
}

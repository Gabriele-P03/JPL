/**
 * Animator: holds the playback state (time, current pose) for one instance of a Model.
 *
 * The Model is immutable and shareable; each animated object owns its own Animator.
 * Every frame: call update(dt), then read globalMatrix(), jointMatrices() and weights().
 */

#ifndef ANIMATOR_GRAPHICS_JPL
#define ANIMATOR_GRAPHICS_JPL

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Model.hpp"

namespace jpl {
    namespace _graphics {
        namespace _mesh {

            class Animator {

                private:

                    const Model& model;
                    const Clip* clip = nullptr;
                    float time = 0.f;
                    bool loop = true;

                    // Current local pose of every node
                    std::vector<glm::vec3> t, s;
                    std::vector<glm::quat> r;
                    std::vector<std::vector<float>> nodeWeights;     // morph weights per node

                    // Computed every update()
                    std::vector<glm::mat4> global;                   // node -> model space
                    std::vector<std::vector<glm::mat4>> joints;      // one array per skin

                    // Recursively computes global matrices from the current local pose
                    void updateNode(int i, const glm::mat4& parent);

                public:

                    explicit Animator(const Model& model);

                    /**
                     * Starts a clip by name. Returns false if the clip does not exist.
                     */
                    bool play(const std::string& name, bool looping = true);

                    void stop() noexcept {
                        this->clip = nullptr;
                    }

                    /**
                     * Advances the animation by dt seconds and recomputes the whole pose.
                     */
                    void update(float dt);

                    // Node -> model space matrix (use it as model matrix for rigid, node-animated meshes)
                    const glm::mat4& globalMatrix(int node) const noexcept {
                        return this->global[node];
                    }

                    // Joint matrices of a skin, ready to be uploaded as a uniform array
                    const std::vector<glm::mat4>& jointMatrices(int skin) const noexcept {
                        return this->joints[skin];
                    }

                    // Current morph weights of a node (empty if the node has no morph targets)
                    const std::vector<float>& weights(int node) const noexcept {
                        return this->nodeWeights[node];
                    }

                    float currentTime() const noexcept {
                        return this->time;
                    }
            };
        }
    }
}

#endif

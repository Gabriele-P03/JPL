/**
 * Runtime representation of a glTF asset: meshes, node hierarchy, skins,
 * animation clips and morph targets.
 *
 * The Model is immutable after loading and can be shared by many Animators.
 */

#ifndef MODEL_GRAPHICS_JPL
#define MODEL_GRAPHICS_JPL

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Mesh.hpp"

namespace jpl {
    namespace _graphics {
        namespace _mesh {

            /*
             * Vertex layout of every Mesh produced by the loader (interleaved, in floats):
             *
             *  static mesh  (8 floats):  x y z | u v | nx ny nz
             *  skinned mesh (16 floats): x y z | u v | nx ny nz | j0 j1 j2 j3 | w0 w1 w2 w3
             *
             * Joint indices are stored as floats (small integers are exactly representable).
             */
            constexpr unsigned int FLOATS_PER_VERTEX_STATIC  = 8;
            constexpr unsigned int FLOATS_PER_VERTEX_SKINNED = 16;
            constexpr unsigned int OFFSET_UV      = 3;
            constexpr unsigned int OFFSET_NORMAL  = 5;
            constexpr unsigned int OFFSET_JOINTS  = 8;
            constexpr unsigned int OFFSET_WEIGHTS = 12;

            // ------------------------------------------------------------------
            // Node hierarchy
            // ------------------------------------------------------------------
            struct Node {
                std::string name;
                int parent = -1;                    // -1 = root
                std::vector<int> children;

                // Rest pose (local TRS)
                glm::vec3 t{0.f};
                glm::quat r{1.f, 0.f, 0.f, 0.f};    // (w, x, y, z)
                glm::vec3 s{1.f};

                std::vector<int> meshes;            // indices into Model::meshes (one per glTF primitive)
                int skin = -1;                      // index into Model::skins, -1 = none
                std::vector<float> weights;         // rest-pose morph weights (empty = no morph)
            };

            // ------------------------------------------------------------------
            // Skinning
            // ------------------------------------------------------------------
            struct Skin {
                std::vector<int> joints;            // node indices
                std::vector<glm::mat4> inverseBind; // one per joint
            };

            // ------------------------------------------------------------------
            // Animation
            // ------------------------------------------------------------------
            enum class Path   { Translation, Rotation, Scale, Weights };
            enum class Interp { Linear, Step, Cubic };

            struct Sampler {
                std::vector<float> times;           // keyframe times (seconds)
                Interp interp = Interp::Linear;

                // Translation / Scale: stored as (x, y, z, 0). Rotation: (x, y, z, w).
                // For Cubic interpolation there are 3 entries per keyframe:
                // in-tangent, value, out-tangent.
                std::vector<glm::vec4> values;

                // Only for Path::Weights. Layout per keyframe:
                //   Linear/Step: N values
                //   Cubic:       3*N values (in-tangents, values, out-tangents)
                std::vector<float> weights;
                std::size_t numTargets = 0;         // N
            };

            struct Channel {
                int node;
                Path path;
                int sampler;                        // index into Clip::samplers
            };

            struct Clip {
                std::string name;
                float duration = 0.f;
                std::vector<Sampler> samplers;
                std::vector<Channel> channels;
            };

            // ------------------------------------------------------------------
            // Morph targets (blend shapes)
            // ------------------------------------------------------------------
            struct MorphTarget {
                std::vector<glm::vec3> dPos;        // per-vertex position delta (empty if absent)
                std::vector<glm::vec3> dNormal;     // per-vertex normal delta (empty if absent)
            };

            // One MorphSet per Mesh*, same index as Model::meshes.
            struct MorphSet {
                std::vector<MorphTarget> targets;   // empty if the primitive has no morph targets
            };

            // ------------------------------------------------------------------
            // Model
            // ------------------------------------------------------------------
            struct Model {
                std::vector<Mesh*> meshes;          // owned by the Model
                std::vector<MorphSet> morphs;       // parallel to meshes
                std::vector<Node> nodes;
                std::vector<int> roots;             // nodes without a parent
                std::vector<Skin> skins;
                std::vector<Clip> clips;

                Model() = default;
                Model(Model&&) noexcept = default;

                // The Model owns raw Mesh pointers: forbid copies and assignment
                Model(const Model&) = delete;
                Model& operator=(const Model&) = delete;
                Model& operator=(Model&&) = delete;

                ~Model();   // deletes every Mesh* (defined in Model.cpp)
            };
        }
    }
}

#endif

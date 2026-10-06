#include "Morph.hpp"

namespace jpl {
    namespace _graphics {
        namespace _mesh {

            void applyMorph(const Mesh& mesh,
                            const MorphSet& ms,
                            const std::vector<float>& w,
                            std::vector<float>& out)
            {
                const float* base = mesh.getVertices();
                const unsigned int stride = mesh.getCoordsPerPoint();   // 8 or 16 floats per vertex

                // Start from the base mesh
                out.assign(base, base + mesh.getSizeVertices());

                for (std::size_t t = 0; t < ms.targets.size() && t < w.size(); ++t) {
                    if (w[t] == 0.f) continue;   // skipping inactive targets saves a lot of work

                    const MorphTarget& mt = ms.targets[t];

                    for (std::size_t v = 0; v < mt.dPos.size(); ++v) {
                        float* p = out.data() + v * stride;                     // position at float 0
                        p[0] += w[t] * mt.dPos[v].x;
                        p[1] += w[t] * mt.dPos[v].y;
                        p[2] += w[t] * mt.dPos[v].z;
                    }

                    for (std::size_t v = 0; v < mt.dNormal.size(); ++v) {
                        float* p = out.data() + v * stride + OFFSET_NORMAL;     // normal at float 5
                        p[0] += w[t] * mt.dNormal[v].x;
                        p[1] += w[t] * mt.dNormal[v].y;
                        p[2] += w[t] * mt.dNormal[v].z;
                    }
                }
            }
        }
    }
}

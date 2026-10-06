/**
 * CPU morph target (blend shape) application.
 *
 * final = base + sum_i( weight_i * delta_i )
 *
 * The result is written into a scratch buffer with the same layout as the Mesh vertex array,
 * ready to be uploaded to a GL_DYNAMIC_DRAW VBO with glBufferSubData.
 * Morphing happens in bind space; skinning is applied afterwards by the vertex shader.
 */

#ifndef MORPH_GRAPHICS_JPL
#define MORPH_GRAPHICS_JPL

#include <vector>

#include "Model.hpp"

namespace jpl {
    namespace _graphics {
        namespace _mesh {

            /**
             * @param mesh  base mesh (its vertices are never modified)
             * @param ms    morph targets of that mesh
             * @param w     current weights, one per target
             * @param out   receives the deformed vertex array (same size/layout as the mesh)
             */
            void applyMorph(const Mesh& mesh,
                            const MorphSet& ms,
                            const std::vector<float>& w,
                            std::vector<float>& out);
        }
    }
}

#endif

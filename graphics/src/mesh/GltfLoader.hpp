/**
 * glTF loader (implemented with fastgltf, see GltfLoader.cpp).
 */

#ifndef GLTF_LOADER_GRAPHICS_JPL
#define GLTF_LOADER_GRAPHICS_JPL

#include <filesystem>

#include "Model.hpp"

namespace jpl {
    namespace _graphics {
        namespace _mesh {

            /**
             * Loads a .gltf / .glb file (fastgltf >= 0.8 API): meshes, node hierarchy,
             * skins, animation clips and morph targets.
             *
             * Throws jpl::IllegalArgumentException if the file cannot be opened or parsed.
             */
            Model loadGltfModel(const std::filesystem::path& path);
        }
    }
}

#endif

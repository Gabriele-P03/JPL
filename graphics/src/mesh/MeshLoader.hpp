/**
 * The mesh loader provides loading function for OBJ file
 * 
 */
#ifndef MESHLOADER_GRAPHICS_JPL
#define MESHLOADER_GRAPHICS_JPL

#include "Mesh.hpp"

namespace jpl{
    namespace _graphics{
        namespace _mesh{

            extern std::vector<std::string>* _split(std::string l, char del);

            extern Mesh* parse(std::istream* f);
        }
    }
}

#endif
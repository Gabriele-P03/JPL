#ifndef CUBE_GRAPHICS_JPL
#define CUBE_GRAPHICS_JPL

#include "Shape.hpp"

namespace jpl{
    namespace _graphics{
        namespace _shapes{

            class Cube : public Shape{

                public:

static constexpr unsigned short SIZE_TEXTURE = 2;
static constexpr unsigned short SIZE_NORMALS = 3;
static constexpr unsigned short VALUES_PER_POINTS = Shape::COORDS_PER_POINT + SIZE_TEXTURE + SIZE_NORMALS;
static constexpr unsigned int POINTS = 24;
static constexpr unsigned int SIZE = POINTS*VALUES_PER_POINTS;

static constexpr float ORTHO_VERTICES[SIZE] = {
    // Posizioni            // UV         // Normali (X, Y, Z)

    // 1. FACCIA DIETRO (Normale: 0, 0, -1)
    0.0f, -1.0f,  0.0f,      0.0f, 0.0f,    0.0f,  0.0f, -1.0f,
   -1.0f, -1.0f,  0.0f,      1.0f, 0.0f,    0.0f,  0.0f, -1.0f,
   -1.0f,  0.0f,  0.0f,      1.0f, 1.0f,    0.0f,  0.0f, -1.0f,
    0.0f,  0.0f,  0.0f,      0.0f, 1.0f,    0.0f,  0.0f, -1.0f,

    // 2. FACCIA FRONTE (Normale: 0, 0, 1)
   -1.0f, -1.0f,  1.0f,      0.0f, 0.0f,    0.0f,  0.0f,  1.0f,
    0.0f, -1.0f,  1.0f,      1.0f, 0.0f,    0.0f,  0.0f,  1.0f,
    0.0f,  0.0f,  1.0f,      1.0f, 1.0f,    0.0f,  0.0f,  1.0f,
   -1.0f,  0.0f,  1.0f,      0.0f, 1.0f,    0.0f,  0.0f,  1.0f,

    // 3. FACCIA SINISTRA (Normale: -1, 0, 0)
   -1.0f, -1.0f,  0.0f,      0.0f, 0.0f,   -1.0f,  0.0f,  0.0f,
   -1.0f, -1.0f,  1.0f,      1.0f, 0.0f,   -1.0f,  0.0f,  0.0f,
   -1.0f,  0.0f,  1.0f,      1.0f, 1.0f,   -1.0f,  0.0f,  0.0f,
   -1.0f,  0.0f,  0.0f,      0.0f, 1.0f,   -1.0f,  0.0f,  0.0f,

    // 4. FACCIA DESTRA (Normale: 1, 0, 0)
    0.0f, -1.0f,  1.0f,      0.0f, 0.0f,    1.0f,  0.0f,  0.0f,
    0.0f, -1.0f,  0.0f,      1.0f, 0.0f,    1.0f,  0.0f,  0.0f,
    0.0f,  0.0f,  0.0f,      1.0f, 1.0f,    1.0f,  0.0f,  0.0f,
    0.0f,  0.0f,  1.0f,      0.0f, 1.0f,    1.0f,  0.0f,  0.0f,

    // 5. FACCIA SOTTO (Normale: 0, -1, 0)
   -1.0f, -1.0f,  0.0f,      0.0f, 1.0f,    0.0f, -1.0f,  0.0f,
    0.0f, -1.0f,  0.0f,      1.0f, 1.0f,    0.0f, -1.0f,  0.0f,
    0.0f, -1.0f,  1.0f,      1.0f, 0.0f,    0.0f, -1.0f,  0.0f,
   -1.0f, -1.0f,  1.0f,      0.0f, 0.0f,    0.0f, -1.0f,  0.0f,

    // 6. FACCIA SOPRA (Normale: 0, 1, 0)
   -1.0f,  0.0f,  1.0f,      0.0f, 0.0f,    0.0f,  1.0f,  0.0f,
    0.0f,  0.0f,  1.0f,      1.0f, 0.0f,    0.0f,  1.0f,  0.0f,
    0.0f,  0.0f,  0.0f,      1.0f, 1.0f,    0.0f,  1.0f,  0.0f,
   -1.0f,  0.0f,  0.0f,      0.0f, 1.0f,    0.0f,  1.0f,  0.0f
};



static constexpr unsigned int SIZE_INDICES = 36;
static constexpr unsigned int ORTHO_INDICES[SIZE_INDICES] = {
    0,  1,  2,     0,  2,  3,   // Faccia 1 (Dietro)
    4,  5,  6,     4,  6,  7,   // Faccia 2 (Fronte)
    8,  9,  10,    8,  10, 11,  // Faccia 3 (Sinistra)
    12, 13, 14,    12, 14, 15,  // Faccia 4 (Destra)
    16, 17, 18,    16, 18, 19,  // Faccia 5 (Sotto)
    20, 21, 22,    20, 22, 23   // Faccia 6 (Sopra)
};

            };
        }
    }
}

#endif
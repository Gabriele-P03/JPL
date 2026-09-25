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

    // 1. FACCIA DIETRO (Normale: 0, 0, -1) - CORRETTA CCW
    0.5f, -0.5f, -0.5f,    1.0f, 0.0f,    0.0f,  0.0f, -1.0f, // 0: Basso-Destra
    -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,    0.0f,  0.0f, -1.0f, // 1: Basso-Sinistra
    -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,    0.0f,  0.0f, -1.0f, // 2: Alto-Sinistra
    0.5f,  0.5f, -0.5f,    1.0f, 1.0f,    0.0f,  0.0f, -1.0f, // 3: Alto-Destra

    // 2. FACCIA FRONTE (Normale: 0, 0, 1) - CORRETTA CCW
    -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,    0.0f,  0.0f,  1.0f, // 4: Basso-Sinistra
    0.5f, -0.5f,  0.5f,    1.0f, 0.0f,    0.0f,  0.0f,  1.0f, // 5: Basso-Destra
    0.5f,  0.5f,  0.5f,    1.0f, 1.0f,    0.0f,  0.0f,  1.0f, // 6: Alto-Destra
    -0.5f,  0.5f,  0.5f,    0.0f, 1.0f,    0.0f,  0.0f,  1.0f, // 7: Alto-Sinistra

    // 3. FACCIA SINISTRA (Normale: -1, 0, 0) - CORRETTA CCW
    -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,   -1.0f,  0.0f,  0.0f, // 8: Basso-Dietro
    -0.5f, -0.5f,  0.5f,    1.0f, 0.0f,   -1.0f,  0.0f,  0.0f, // 9: Basso-Fronte
    -0.5f,  0.5f,  0.5f,    1.0f, 1.0f,   -1.0f,  0.0f,  0.0f, // 10: Alto-Fronte
    -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,   -1.0f,  0.0f,  0.0f, // 11: Alto-Dietro

    // 4. FACCIA DESTRA (Normale: 1, 0, 0) - CORRETTA CCW
    0.5f, -0.5f,  0.5f,    0.0f, 0.0f,    1.0f,  0.0f,  0.0f, // 12: Basso-Fronte
    0.5f, -0.5f, -0.5f,    1.0f, 0.0f,    1.0f,  0.0f,  0.0f, // 13: Basso-Dietro
    0.5f,  0.5f, -0.5f,    1.0f, 1.0f,    1.0f,  0.0f,  0.0f, // 14: Alto-Dietro
    0.5f,  0.5f,  0.5f,    0.0f, 1.0f,    1.0f,  0.0f,  0.0f, // 15: Alto-Fronte

    // 5. FACCIA SOTTO (Normale: 0, -1, 0) - CORRETTA CCW
    -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,    0.0f, -1.0f,  0.0f, // 16: Basso-Dietro
    0.5f, -0.5f, -0.5f,    1.0f, 0.0f,    0.0f, -1.0f,  0.0f, // 17: Basso-Dietro-DX
    0.5f, -0.5f,  0.5f,    1.0f, 1.0f,    0.0f, -1.0f,  0.0f, // 18: Basso-Fronte-DX
    -0.5f, -0.5f,  0.5f,    0.0f, 1.0f,    0.0f, -1.0f,  0.0f, // 19: Basso-Fronte

    // 6. FACCIA SOPRA (Normale: 0, 1, 0) - CORRETTA CCW
    -0.5f,  0.5f,  0.5f,    0.0f, 0.0f,    0.0f,  1.0f,  0.0f, // 20: Alto-Fronte
    0.5f,  0.5f,  0.5f,    1.0f, 0.0f,    0.0f,  1.0f,  0.0f, // 21: Alto-Fronte-DX
    0.5f,  0.5f, -0.5f,    1.0f, 1.0f,    0.0f,  1.0f,  0.0f, // 22: Alto-Dietro-DX
    -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,    0.0f,  1.0f,  0.0f  // 23: Alto-Dietro
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
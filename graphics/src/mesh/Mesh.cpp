#include "Mesh.hpp"

jpl::_graphics::_mesh::Mesh::Mesh(const float* vertices, unsigned int sizeVertices, unsigned int offsetTexture, const unsigned int* indices, unsigned int sizeIndices, unsigned int coordsPerPoint, bool normalized)
    : vertices(vertices), sizeVertices(sizeVertices), offsetTexture(offsetTexture), indices(indices), sizeIndices(sizeIndices), coordsPerPoint(coordsPerPoint), normalized(normalized){
}

jpl::_graphics::_mesh::Mesh::~Mesh(){
    delete[] this->vertices;
    delete[] this->indices;
}

const jpl::_graphics::_mesh::Mesh* const jpl::_graphics::_mesh::TRIANGLE = new jpl::_graphics::_mesh::Mesh(
    jpl::_graphics::_shapes::Triangle::ORTHO_VERTICES,
    jpl::_graphics::_shapes::Triangle::SIZE,
    jpl::_graphics::_shapes::Triangle::COORDS_PER_POINT,
    jpl::_graphics::_shapes::Triangle::ORTHO_INDICES,
    jpl::_graphics::_shapes::Triangle::SIZE_INDICES,
    jpl::_graphics::_shapes::Triangle::VALUES_PER_POINTS,
    false
);

const jpl::_graphics::_mesh::Mesh* const jpl::_graphics::_mesh::QUAD = new jpl::_graphics::_mesh::Mesh(
    jpl::_graphics::_shapes::Quad::ORTHO_VERTICES,
    jpl::_graphics::_shapes::Quad::SIZE,
    jpl::_graphics::_shapes::Quad::COORDS_PER_POINT,
    jpl::_graphics::_shapes::Quad::ORTHO_INDICES,
    jpl::_graphics::_shapes::Quad::SIZE_INDICES,
    jpl::_graphics::_shapes::Quad::VALUES_PER_POINTS,
    false
);

const jpl::_graphics::_mesh::Mesh* const jpl::_graphics::_mesh::CUBE = new jpl::_graphics::_mesh::Mesh(
    jpl::_graphics::_shapes::Cube::ORTHO_VERTICES,
    jpl::_graphics::_shapes::Cube::SIZE,
    jpl::_graphics::_shapes::Cube::COORDS_PER_POINT,
    jpl::_graphics::_shapes::Cube::ORTHO_INDICES,
    jpl::_graphics::_shapes::Cube::SIZE_INDICES,
    jpl::_graphics::_shapes::Cube::VALUES_PER_POINTS,
    false
);
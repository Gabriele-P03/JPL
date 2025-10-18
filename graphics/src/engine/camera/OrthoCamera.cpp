#include "OrthoCamera.hpp"

jpl::_graphics::_engine::_camera::OrthoCamera::OrthoCamera(glm::vec2 pos)
    : jpl::_graphics::_engine::_camera::BaseCamera(
        glm::vec3(pos.x, pos.y, -2.0f),
        glm::normalize(glm::vec3(pos.x, pos.y, -2.0f))
    ){
}

jpl::_graphics::_engine::_camera::OrthoCamera::OrthoCamera()
    : jpl::_graphics::_engine::_camera::OrthoCamera(glm::vec2(0.0f)){
}

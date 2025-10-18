#include "PerspectiveCamera.hpp"

jpl::_graphics::_engine::_camera::PerspectiveCamera::PerspectiveCamera(glm::vec3 pos, glm::vec3 dir)
    : jpl::_graphics::_engine::_camera::BaseCamera(
        glm::vec3(pos.x, pos.y, 2.0f),
        glm::normalize(glm::vec3(pos.x, pos.y, 2.0f))
    ){
    this->yaw = 0;
    this->pitch = 0;
    this->q = glm::quat();
}

jpl::_graphics::_engine::_camera::PerspectiveCamera::PerspectiveCamera()
    : jpl::_graphics::_engine::_camera::PerspectiveCamera(
        glm::vec3(0.0f, 0.0f, 2.0f), //Position
        glm::vec3(  //Direction
            glm::normalize(
                glm::vec3(0.0f, 0.0f, 2.0f) - glm::vec3(0.0f, 0.0f, 0.0f)
            )
        )
    ){
}

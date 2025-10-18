#include "BaseCamera.hpp"

jpl::_graphics::_engine::_camera::BaseCamera::BaseCamera(glm::vec3 pos, glm::vec3 dir){
    this->pos = new glm::vec3(pos);
    this->direction = new glm::vec3(dir);
    this->fov = glm::radians(90.0f);
    this->aspectRatio = 4.0f/3.0f;
    this->f = 100.0f;
    this->n = 0.1f;
}

jpl::_graphics::_engine::_camera::BaseCamera::~BaseCamera(){
    delete this->pos;
    delete this->direction;
}
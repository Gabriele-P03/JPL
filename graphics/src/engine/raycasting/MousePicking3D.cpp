#include "MousePicking3D.hpp"

jpl::_graphics::_engine::_raycasting::Ray jpl::_graphics::_engine::_raycasting::getMouseRay(
    float mouseX, float mouseY, float widthWindow, float heightWindow, const glm::mat4 &view, const glm::mat4 &projection) {

    float x = (2.0f * mouseX) / widthWindow - 1.0f;
    float y = 1.0f - (2.0f * mouseY) / heightWindow;
    glm::vec4 rayClip = glm::vec4(x, y, -1.0f, 1.0f);
    glm::vec4 rayEye = glm::inverse(projection) * rayClip;
    rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);
    glm::vec3 rayWorld = glm::vec3(glm::inverse(view) * rayEye);
    rayWorld = glm::normalize(rayWorld);
    glm::vec3 cameraPos = glm::vec3(glm::inverse(view)[3]);
    return { cameraPos, rayWorld };
}

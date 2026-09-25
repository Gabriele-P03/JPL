
#ifndef FRUSTUMCULLING_HPP
#define FRUSTUMCULLING_HPP
#include <glm/glm.hpp>

namespace jpl::_graphics::_engine::_camera {

    struct FrustumPlane {
        glm::vec3 normal;
        float distance;

        void normalize() {
            const float length = glm::length(normal);
            normal /= length;
            distance /= length;
        }

        float getSignedDistance(const glm::vec3& point) const {
            return glm::dot(normal, point) + distance;
        }
    };

    struct CameraFrustum {
        FrustumPlane planes[6];

        void update(const glm::mat4& viewProjection) {
            glm::mat4 m = glm::transpose(viewProjection);
            planes[0].normal   = glm::vec3(m[3] + m[0]);    //Left
            planes[0].distance = m[3].w + m[0].w;
            planes[1].normal   = glm::vec3(m[3] - m[0]);    //Right
            planes[1].distance = m[3].w - m[0].w;
            planes[2].normal   = glm::vec3(m[3] + m[1]);    //Bottom
            planes[2].distance = m[3].w + m[1].w;
            planes[3].normal   = glm::vec3(m[3] - m[1]);    //Top
            planes[3].distance = m[3].w - m[1].w;
            planes[4].normal   = glm::vec3(m[3] + m[2]);    //Near
            planes[4].distance = m[3].w + m[2].w;
            planes[5].normal   = glm::vec3(m[3] - m[2]);    //Far
            planes[5].distance = m[3].w - m[2].w;
            for (int i = 0; i < 6; ++i) {
                planes[i].normalize();
            }
        }
        bool isPointVisible(const glm::vec3& center, float radius = 0.86f) const {
            for (int i = 0; i < 6; ++i) {
                if (planes[i].getSignedDistance(center) < -radius) {
                    return false;
                }
            }
            return true;
        }
    };

}
#endif

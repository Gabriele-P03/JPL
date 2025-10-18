#include "BaseCamera.hpp"

#include <glm/gtc/quaternion.hpp>
#include <iostream>

namespace jpl{
    namespace _graphics{
        namespace _engine{
            namespace _camera{

                class PerspectiveCamera : public BaseCamera{

                    protected:

                        float yaw, pitch;
                        glm::quat q;

                    public:
                        PerspectiveCamera();
                        PerspectiveCamera(glm::vec3 pos, glm::vec3 dir);

                        virtual void translate(float x, float y, float z) noexcept{
                            this->pos->y += y;
                            this->pos->x += cos(glm::radians(this->yaw))*x + sin(glm::radians(this->pitch))*z;
                            this->pos->z += sin(glm::radians(this->yaw))*x + cos(glm::radians(this->pitch))*z;
                        }

                        virtual void rotate(float xoffset, float yoffset) noexcept{
                            this->yaw += xoffset;
                            this->pitch += yoffset;
                            this->q = glm::angleAxis(glm::radians(-this->pitch), glm::vec3(1.0f, 0.0f, 0.0f));
                            this->q *= glm::angleAxis(glm::radians(this->yaw), glm::vec3(0.0f, 1.0f, 0.0f));
                        }

                        virtual glm::mat4 getLookAt(glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f)) const noexcept override{
                            return glm::translate(glm::mat4_cast(this->q), *this->pos);
                            //glm::quat q = glm::angleAxis(glm::radians(-this->pitch), glm::vec3(1.0f, 0.0f, 0.0f));
                            //q *= glm::angleAxis(glm::radians(this->yaw), glm::vec3(0.0f, 1.0f, 0.0f));
                            //return glm::translate(glm::mat4_cast(q), *this->pos);
                        }

                        virtual glm::mat4 getProjection() const noexcept override{
                            return glm::perspective(
                                this->fov,
                                BaseCamera::aspectRatio,
                                this->n,
                                this->f
                            );
                        }

                        /**
                         * Update Camera's right and up vectors.
                         * This function should be called only by a perspective camera
                         */
                        virtual void updateCameraRightAndUp() noexcept override{
                            this->up = glm::normalize(
                                glm::cross(
                                    glm::vec3(0.0f, 1.0f, 0.0f), //Standard up vector
                                    *this->direction
                                )
                            );
                            this->right = glm::cross(*this->direction, this->up); //Dir and up are already normalized
                        }
                };

            }
        }
    }
}
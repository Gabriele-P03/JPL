#include "BaseCamera.hpp"

#include <glm/gtc/quaternion.hpp>
#include <iostream>
#include "FrustumCulling.hpp"

namespace jpl{
    namespace _graphics{
        namespace _engine{
            namespace _camera{

                class PerspectiveCamera : public BaseCamera{

                    protected:

                        float yaw = -90.0f, pitch;
                        glm::quat q;
                        CameraFrustum frustum;

                    public:
                        PerspectiveCamera();
                        PerspectiveCamera(glm::vec3 pos, glm::vec3 dir);

                    virtual void rotate(float xoffset, float yoffset) noexcept {
                        this->yaw   += xoffset;
                        this->pitch += yoffset;

                        if (this->pitch > 89.0f)  this->pitch = 89.0f;
                        if (this->pitch < -89.0f) this->pitch = -89.0f;

                        // 1. Calcola il quaternione di rotazione pulito
                        glm::quat qYaw   = glm::angleAxis(glm::radians(this->yaw), glm::vec3(0.0f, 1.0f, 0.0f));
                        glm::quat qPitch = glm::angleAxis(glm::radians(-this->pitch), glm::vec3(1.0f, 0.0f, 0.0f));
                        this->q = qYaw * qPitch;

                        // 2. ESTRAI IL VETTORE DIREZIONE DIRETTAMENTE DAL QUATERNIONE
                        // Questo garantisce che la direzione del movimento sia SEMPRE identica a quella visiva
                        *this->direction = this->q * glm::vec3(0.0f, 0.0f, -1.0f);
                    }

                    virtual void translate(float x, float y, float z) noexcept {
                        // Ora this->direction è matematicamente fuso con la matrice di View
                        glm::vec3 cameraFront = glm::normalize(glm::vec3(this->direction->x, 0.0f, this->direction->z));
                        glm::vec3 cameraRight = glm::normalize(glm::cross(cameraFront, glm::vec3(0.0f, 1.0f, 0.0f)));

                        // Se un asse risulta invertito rispetto ai tasti, ti basta cambiare il segno + o - qui sotto
                        *this->pos += cameraRight * x;
                        this->pos->y += y;
                        *this->pos += cameraFront * z;
                    }

                        virtual glm::mat4 getLookAt(glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f)) const noexcept override{
                            glm::mat4 rotation = glm::mat4_cast(glm::conjugate(this->q));
                            glm::mat4 translation = glm::translate(glm::mat4(1.0f), -(*this->pos));
                            return rotation * translation;
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
                            this->frustum.update(this->getProjection() * this->getLookAt());
                        }

                        CameraFrustum* getFrustum() noexcept {
                            return &this->frustum;
                        }
                };

            }
        }
    }
}
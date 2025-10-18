/**
 * Base class of camera object
 */

#ifndef BASECAMERA_GRAPHICS_JPL
#define BASECAMERA_GRAPHICS_JPL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace jpl{
    namespace _graphics{
        
        namespace _metrics{

            //Window width
            extern unsigned int width;
            //Window height
            extern unsigned int height;
        }
        
        namespace _engine{
            namespace _camera{

                enum MOVEMENT{
                    FRONT,
                    BACK,
                    LEFT,
                    RIGHT,
                    UP,
                    BOTTOM
                };

                class BaseCamera{

                    public:

                        glm::vec3* pos;
                        glm::vec3* direction;
                        glm::vec3 right, up;

                        float fov, aspectRatio, n, f;

                        BaseCamera(glm::vec3 pos, glm::vec3 dir);

                        virtual ~BaseCamera();

                    public:

                        glm::vec3* getPos() const noexcept{
                            return this->pos;
                        }

                        glm::vec3* getDirection() const noexcept{
                            return this->direction;
                        }

                        glm::vec3 getRight() const noexcept{
                            return this->right;
                        }
                        glm::vec3 getUp() const noexcept{
                            return this->up;
                        }

                        

                        void setFOV(float fov) noexcept {
                            this->fov = fov;
                        }
                        float getFOV() const noexcept{
                            return this->fov;
                        }
                        void setAspectRatio(float ar) noexcept{
                            this->aspectRatio = ar;
                        }
                        float getAspectRatio() const noexcept{
                            return this->aspectRatio;
                        }
                        void getNear(float n) noexcept{
                            this->n = n;
                        }
                        float getNear() const noexcept{
                            return this->n;
                        }
                        void setFar(float f) noexcept{
                            this->f = f;
                        }
                        float getFar() const noexcept{
                            return this->f;
                        }

                        virtual void translate(float x, float y, float z) noexcept{
                            this->pos->x += x;
                            this->pos->y += y;
                            this->pos->z += z;
                        }

                        virtual void rotate(float yaw, float pitch) noexcept = 0;

                        virtual glm::mat4 getLookAt(glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f)) const noexcept = 0;
                        virtual glm::mat4 getProjection() const noexcept = 0;

                        /**
                         * Update Camera's right and up vectors.
                         * This function should be called only by a perspective camera
                         */
                        virtual void updateCameraRightAndUp() noexcept = 0;
                };
            }
        }
    }
}

#endif
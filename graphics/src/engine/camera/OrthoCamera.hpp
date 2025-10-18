#include "BaseCamera.hpp"

namespace jpl{
    namespace _graphics{
        namespace _engine{
            namespace _camera{

                class OrthoCamera : public BaseCamera{

                    public:
                        OrthoCamera();
                        OrthoCamera(glm::vec2 pos);

                        virtual glm::mat4 getLookAt(glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f)) const noexcept{
                            glm::mat4 b = glm::translate(glm::mat4(1.0f), -*this->pos);
                            return b;
                        }

                        virtual glm::mat4 getProjection() const noexcept{
                            return glm::ortho(0.0f, (float)::jpl::_graphics::_metrics::width, 0.0f, (float)::jpl::_graphics::_metrics::height, this->n, this->f);
                        }

                        virtual void updateCameraRightAndUp() noexcept override{}
                };

            }
        }
    }
}
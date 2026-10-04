/**
 *  Mouse Picking 3D exploits RayCasting in order to detect the nearest object that player is aiming
 */

#ifndef GRAPHICS_MOUSEPICKING3D_HPP
#define GRAPHICS_MOUSEPICKING3D_HPP

#include "Ray.hpp"

namespace jpl::_graphics::_engine::_raycasting {

    constexpr float step = 0.1f;
    constexpr float maxDistance = 3.0f;

    extern Ray getMouseRay(
        float mouseX, float mouseY, float widthWindow, float heightWindow, const glm::mat4& view, const glm::mat4& projection
    );

    template<typename T, typename W>
    struct MP3DResolver {

        struct MP3DResolverResult {
            T t;
            glm::ivec3 pos;
        };

        using BlockGetterFunction = T(W::*)(const glm::ivec3&);

        /**
         * Retrieve which block player is aiming to
         * @param ray
         * @param world your own world
         * @param blockGetter function of world class which returns blocks by glm::ivec3
         * @param toAvoid object that will be compared with result, in case of difference, the block is returned
         * @return MP3DResolverResult(block, blockPos); in case of no blocks detected MP3DResolverResult(toAvoid, ray.origin)
         */
        MP3DResolverResult getBlockAiming(const Ray& ray, W& world, BlockGetterFunction blockGetter, const T& toAvoid) {
            for (float t = 0; t < maxDistance; t += step) {
                glm::vec3 checkPos = ray.origin + t * ray.direction;
                glm::ivec3 blockpos = glm::ivec3(
                    std::floor(checkPos.x),
                    std::floor(checkPos.y),
                    std::floor(checkPos.z)
                );
                T res = (world.*blockGetter)(blockpos);
                if (res != toAvoid) {
                    return MP3DResolverResult{res, glm::ivec3(blockpos)};
                }
            }
            return MP3DResolverResult{toAvoid, glm::ivec3(ray.origin)};
        }
    };

}


#endif
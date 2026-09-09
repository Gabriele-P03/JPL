#ifndef PROJECTION_PHYSICS_JPL
#define PROJECTION_PHYSICS_JPL

namespace jpl{
    namespace _physics{
        struct Projection {
            float min;
            float max;

            bool Overlaps(const Projection& other) const {
                return min <= other.max && max <= other.min;
            }

            float GetOverlap(const Projection& other) const {
                return glm::min(max, other.max) - glm::max(min, other.min);
            }
        };
    }
}

#endif
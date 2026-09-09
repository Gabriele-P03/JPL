#ifndef SPATIAL_HASH_PHYSICS_JPL
#define SPATIAL_HASH_PHYSICS_JPL

#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include <unordered_map>
#include <vector>
#include "../rigidbody/RigidBody.hpp"
#include "../collider/aabb.hpp"

namespace jpl{
    namespace _physics {

        class SpatialHash {
        public:

            SpatialHash(float cellSize = 4.0f) : m_cellSize(cellSize) {}

            void Clear() {
                m_grid.clear();
            }

            void Insert(IRigidBody* body) {
                AABB box = body->GetWorldAABB();
                glm::ivec3 minCell = GetCellCoords(box.min);
                glm::ivec3 maxCell = GetCellCoords(box.max);
                for (int x = minCell.x; x <= maxCell.x; ++x) {
                    for (int y = minCell.y; y <= maxCell.y; ++y) {
                        for (int z = minCell.z; z <= maxCell.z; ++z) {
                            size_t hash = ComputeHash(x, y, z);
                            m_grid[hash].push_back(body);
                        }
                    }
                }
            }

            std::vector<IRigidBody*> GetCandidates(IRigidBody* body) {
                std::vector<IRigidBody*> candidates;
                AABB box = body->GetWorldAABB();
                glm::ivec3 minCell = GetCellCoords(box.min);
                glm::ivec3 maxCell = GetCellCoords(box.max);
                for (int x = minCell.x; x <= maxCell.x; ++x) {
                    for (int y = minCell.y; y <= maxCell.y; ++y) {
                        for (int z = minCell.z; z <= maxCell.z; ++z) {
                            size_t hash = ComputeHash(x, y, z);
                            
                            auto it = m_grid.find(hash);
                            if (it != m_grid.end()) {
                                for (IRigidBody* neighbor : it->second) {
                                    // Evitiamo di inserire l'oggetto stesso
                                    if (neighbor != body) {
                                        candidates.push_back(neighbor);
                                    }
                                }
                            }
                        }
                    }
                }
                return candidates;
            }

        private:

            float m_cellSize;

            glm::ivec3 GetCellCoords(const glm::vec3& point) const {
                return glm::ivec3(
                    static_cast<int>(glm::floor(point.x / m_cellSize)),
                    static_cast<int>(glm::floor(point.y / m_cellSize)),
                    static_cast<int>(glm::floor(point.z / m_cellSize))
                );
            }
            
            size_t ComputeHash(int x, int y, int z) const {
                size_t hashX = static_cast<size_t>(x) * 73856093;
                size_t hashY = static_cast<size_t>(y) * 19349663;
                size_t hashZ = static_cast<size_t>(z) * 83492791;
                return hashX ^ hashY ^ hashZ;
            }

            std::unordered_map<size_t, std::vector<IRigidBody*>> m_grid;
        };

    }
}

#endif
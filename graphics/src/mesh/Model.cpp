#include "Model.hpp"

namespace jpl {
    namespace _graphics {
        namespace _mesh {

            Model::~Model() {
                // The Model owns the Meshes created by the loader
                for (Mesh* m : this->meshes) delete m;
            }
        }
    }
}

#pragma once

// #ifndef ZZ_USEPCH
#include <cstddef>
#include <vector>
// #endif

#include "glm/glm.hpp"

namespace zz {
    struct mesh_buffer {
        std::vector<float> verts {};
        std::vector<size_t> idxs {};
        size_t num_faces();
        size_t num_tris ();
        float* buffer   ();

        glm::vec3 operator[](size_t);
    };

    namespace component {
        struct mesh {
        public:
            mesh(std::vector<glm::vec3>);
            mesh(mesh_buffer);
            ~mesh();

            mesh_buffer buffer();
        private:
            std::vector<glm::vec3> verts {};
            std::vector<size_t> indices {};
        };
    }
}
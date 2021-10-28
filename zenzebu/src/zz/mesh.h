#pragma once

// #ifndef ZZ_USEPCH
#include <cstddef>
#include <vector>
// #endif

#include "glm/glm.hpp"

namespace zz {
    namespace component {
        struct mesh {
        public:
            mesh(std::vector<glm::vec3>);
            ~mesh();

            std::vector<glm::vec3> verts {};
            std::vector<size_t> indices {};
            float *vert_buffer();
            size_t *idx_buffer();
            float *vert_buffer_all();

            uint vao, vbo;
            void invalidate();

            enum {
                draw_static, draw_dynamic
            } type;
            bool should_create_buffers_on_init();
        };
    }
}

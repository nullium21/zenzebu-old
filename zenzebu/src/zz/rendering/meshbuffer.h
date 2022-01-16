#pragma once

extern "C" {
#include "glad/glad.h"
}

#include <vector>
#include "glm/glm.hpp"
#include "gl.h"

namespace zz::render {
    constexpr GLenum to_gl_enum(draw_type dt) {
        switch (dt) {
            case draw_type::static_draw : return GL_STATIC_DRAW;
            case draw_type::dynamic_draw: return GL_DYNAMIC_DRAW;
            case draw_type::stream      : return GL_STREAM_DRAW;
        }
    }

    template<class T = glm::vec3>
    class meshbuffer {
    public:
        meshbuffer(T *data, int n_data, int *idxs, int n_idxs, draw_type dt = draw_type::static_draw): drawt(dt) {
            init(data, n_data, idxs, n_idxs);
        }

        meshbuffer(std::vector<T> data, std::vector<int> idxs, draw_type dt = draw_type::static_draw): drawt(dt) {
            init(data.data(), data.size(), idxs.data(), idxs.size());
        }

        ~meshbuffer() {
            if (vao != -1) glDeleteVertexArrays(1, (const unsigned int *) &vao);
            if (vbo != -1) glDeleteBuffers(     1, (const unsigned int *) &vbo);
            if (ebo != -1) glDeleteBuffers(     1, (const unsigned int *) &ebo);

            vao = vbo = ebo = -1;
        }

        void use() { glBindVertexArray(vao); }
    private:
        int vao = -1;
        int vbo = -1;
        int ebo = -1;

        T *data;
        int data_count;

        int *indices;
        int idx_count;

        draw_type drawt;

        void init(T *d, int nd, int *i, int ni) {
            glGenVertexArrays(1, (unsigned int *) &vao);
            glBindVertexArray(vao);

            glGenBuffers(1, (unsigned int *) &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, nd*sizeof(T), d, to_gl_enum(drawt));

            glGenBuffers(1, (unsigned int *) &ebo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, ni*sizeof(int), i, to_gl_enum(drawt));
        }
    };
}
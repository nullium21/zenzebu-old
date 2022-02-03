#pragma once

#include <string>

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

namespace zz::render {
    struct textured_vert {
        glm::vec3 pos;
        glm::vec2 tex;
    };

    class texture {
    public:
        texture(std::string filename, bool gen_mipmaps = true);
        texture(int width, int height, unsigned char *data, bool gen_mipmaps = true);

        ~texture();

        void use() const;

        int unit = 0;
    private:
        int id = -1;

        void init(int w, int h, unsigned char *data, bool gen_mipmaps);
    };
}

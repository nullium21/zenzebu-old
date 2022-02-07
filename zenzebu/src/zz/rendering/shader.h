#pragma once

#include <string>
#include <istream>
#include <vector>
#include <variant>

#include "gl.h"
#include "texture.h"
#include "datatype.h"
#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

namespace zz::render {
    struct attribute {
        std::string name;
        datatype::datatype type;

        enum {
            vertex_in, fragment_in, fragment_out, uniform
        } attr_type;
    };

    struct shader_code {
        std::string prefix;
        std::string main;
        std::string postfix;
    };

    struct shader_param {
        std::string name;
        datatype::datatype type;

        std::variant<
            glm::vec<1, float>, glm::vec<2, float>, glm::vec<3, float>, glm::vec<4, float>,
            glm::vec<1,   int>, glm::vec<2,   int>, glm::vec<3,   int>, glm::vec<4,   int>,
            glm::vec<1,  uint>, glm::vec<2,  uint>, glm::vec<3,  uint>, glm::vec<4,  uint>,

            glm::mat<2, 2, float>, glm::mat<2, 3, float>, glm::mat<2, 4, float>,
            glm::mat<3, 2, float>, glm::mat<3, 3, float>, glm::mat<3, 4, float>,
            glm::mat<4, 2, float>, glm::mat<4, 3, float>, glm::mat<4, 4, float>,

            texture *
        > value;
    };

    struct shader_params_holder {
        std::vector<shader_param> params;

        shader_params_holder(std::vector<shader_param> lst) : params(lst) {}
    };

    class shader {
    public:
        shader(shader_code &vert, shader_code &frag, std::vector<attribute> attrs);

        bool is_initialized();

        ~shader();

        void use();

        #include "generated/vec-uniforms.lua.h"
        #include "generated/mat-uniforms.lua.h"

        void texture(std::string name, int unit, const texture &tex);

        void uniforms(shader_params_holder const &params);

        int get_id();

        void apply_attrs();

        std::vector<attribute> &get_attrs();

        bool has_attr(std::string name, datatype::datatype type);
    private:
        void init(std::string &vert, std::string &frag);

        int id = -1;
        std::vector<attribute> attrs = {};

        int vertex_stride = 0;
    };
}

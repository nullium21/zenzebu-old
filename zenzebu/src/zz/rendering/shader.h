#pragma once

#include <string>
#include <istream>
#include <vector>
#include <variant>

#include "gl.h"
#include "texture.h"
#include "glm/glm.hpp"

namespace zz::render {
    struct attribute {
        std::string name;
        data_type type;

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
        data_type type;

        std::variant<
            glm::vec<1, float>, glm::vec<2, float>, glm::vec<3, float>, glm::vec<4, float>,
            glm::vec<1,   int>, glm::vec<2,   int>, glm::vec<3,   int>, glm::vec<4,   int>,
            glm::vec<1,  uint>, glm::vec<2,  uint>, glm::vec<3,  uint>, glm::vec<4,  uint>,

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

        void uniform(std::string name, float x);
        void uniform(std::string name, int x);
        void uniform(std::string name, uint x);

        void uniform(std::string name, float x, float y);
        void uniform(std::string name, int x, int y);
        void uniform(std::string name, uint x, uint y);

        void uniform(std::string name, float x, float y, float z);
        void uniform(std::string name, int x, int y, int z);
        void uniform(std::string name, uint x, uint y, uint z);

        void uniform(std::string name, float x, float y, float z, float w);
        void uniform(std::string name, int x, int y, int z, int w);
        void uniform(std::string name, uint x, uint y, uint z, uint w);

        void texture(std::string name, int unit, const texture &tex);

        void uniforms(shader_params_holder const &params);

        int get_id();

        void apply_attrs();

        std::vector<attribute> &get_attrs();

        bool has_attr(std::string name, data_type type);
    private:
        void init(std::string &vert, std::string &frag);

        int id = -1;
        std::vector<attribute> attrs = {};

        int vertex_stride = 0;
    };
}
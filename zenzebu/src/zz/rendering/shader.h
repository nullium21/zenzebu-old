#pragma once

#include <string>
#include <istream>
#include <vector>

#include "gl.h"

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

        int get_id();

        void apply_attrs();
    private:
        void init(std::string &vert, std::string &frag);

        int id = -1;
        std::vector<attribute> attrs = {};

        int vertex_stride = 0;
    };
}
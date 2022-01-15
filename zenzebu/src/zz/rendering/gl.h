#pragma once

#include <string>

namespace zz::render {
    using uint = unsigned int;

    enum class shader_type: unsigned int {
        vertex,
        fragment
    };

    enum class draw_type: unsigned int {
        static_draw,
        dynamic_draw,
        stream
    };

    class opengl {
        opengl() = delete;
        ~opengl() = delete;

    public:
        static void load();

        static int compile_shader(shader_type type, std::string &src);
        static int create_program(int vert, int frag);

        static bool shader_compile_ok(int shad);
        static bool program_create_ok(int prog);

        static std::string shader_compile_error(int shad);
        static std::string program_create_error(int prog);

        static void use_program(int prog);

        static int create_vbo(float *data, unsigned int count, draw_type type, int vao = -1);
        static int create_vao();
        static int create_ebo(unsigned int *data, unsigned int count, draw_type type, int vao = -1);

        static void use_vbo(int vbo);
        static void use_ebo(int ebo);
        static void use_vao(int vao);

        static void set_uniform(int prog, std::string name, float x);
        static void set_uniform(int prog, std::string name,   int x);
        static void set_uniform(int prog, std::string name,  uint x);

        static void set_uniform(int prog, std::string name, float x, float y);
        static void set_uniform(int prog, std::string name,   int x,   int y);
        static void set_uniform(int prog, std::string name,  uint x,  uint y);

        static void set_uniform(int prog, std::string name, float x, float y, float z);
        static void set_uniform(int prog, std::string name,   int x,   int y,   int z);
        static void set_uniform(int prog, std::string name,  uint x,  uint y,  uint z);

        static void set_uniform(int prog, std::string name, float x, float y, float z, float w);
        static void set_uniform(int prog, std::string name,   int x,   int y,   int z,   int w);
        static void set_uniform(int prog, std::string name,  uint x,  uint y,  uint z,  uint w);
    };
}
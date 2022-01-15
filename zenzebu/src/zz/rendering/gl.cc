extern "C" {
#include <glad/glad.h>
#include "GLFW/glfw3.h"
}

#include "gl.h"

#define GET_STATUS(id, type) int isok; glGetShaderiv(id, type, &isok); return isok
#define GET_LOG(id, fn) int len; glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len); std::string log; log.reserve(len); fn(id, len, nullptr, log.data()); return log
#define SET_UNIFORM(type, ...) use_program(prog); int loc = glGetUniformLocation(prog, name.c_str()); glUniform##type(loc, __VA_ARGS__)

using namespace zz::render;

constexpr GLenum to_gl_enum(shader_type st) {
    switch (st) {
        case shader_type::fragment: return GL_FRAGMENT_SHADER;
        case shader_type::vertex  : return GL_VERTEX_SHADER;
    }
}

constexpr GLenum to_gl_enum(draw_type dt) {
    switch (dt) {
        case draw_type::static_draw : return GL_STATIC_DRAW;
        case draw_type::dynamic_draw: return GL_DYNAMIC_DRAW;
        case draw_type::stream      : return GL_STREAM_DRAW;
    }
}

void opengl::load() {
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}

int opengl::compile_shader(shader_type type, std::string &src) {
    int ret = glCreateShader(to_gl_enum(type));

    const char *src_data = src.c_str();
    int src_len          = src.length();

    glShaderSource(ret, 1, &src_data, &src_len);
    glCompileShader(ret);

    return ret;
}

int opengl::create_program(int vert, int frag) {
    int ret = glCreateProgram();

    glAttachShader(ret, vert);
    glAttachShader(ret, frag);

    glLinkProgram(ret);

    return ret;
}

bool opengl::shader_compile_ok(int shad) {
    GET_STATUS(shad, GL_COMPILE_STATUS);
}

bool opengl::program_create_ok(int prog) {
    GET_STATUS(prog, GL_LINK_STATUS);
}

std::string opengl::shader_compile_error(int shad) {
    GET_LOG(shad, glGetShaderInfoLog);
}

std::string opengl::program_create_error(int prog) {
    GET_LOG(prog, glGetProgramInfoLog);
}

void opengl::use_program(int prog) {
    glUseProgram(prog);
}

int opengl::create_vbo(float *data, unsigned int count, draw_type type, int vao) {
    unsigned int vbo;
    glGenBuffers(1, &vbo);

    if (vao != -1) glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, count, data, to_gl_enum(type));

    return vbo;
}

int opengl::create_vao() {
    unsigned int vao;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    return vao;
}

int opengl::create_ebo(unsigned int *data, unsigned int count, draw_type type, int vao) {
    unsigned int ebo;
    glGenBuffers(1, &ebo);

    if (vao != -1) glBindVertexArray(vao);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count, data, to_gl_enum(type));

    return ebo;
}

void opengl::use_vbo(int vbo) {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
}

void opengl::use_vao(int vao) {
    glBindVertexArray(vao);
}

void opengl::use_ebo(int ebo) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
}

void opengl::set_uniform(int prog, std::string name, float x) {
    SET_UNIFORM(1f, x);
}

void opengl::set_uniform(int prog, std::string name, int x) {
    SET_UNIFORM(1i, x);
}

void opengl::set_uniform(int prog, std::string name, uint x) {
    SET_UNIFORM(1ui, x);
}

void opengl::set_uniform(int prog, std::string name, float x, float y) {
    SET_UNIFORM(2f, x, y);
}

void opengl::set_uniform(int prog, std::string name, int x, int y) {
    SET_UNIFORM(2i, x, y);
}

void opengl::set_uniform(int prog, std::string name, uint x, uint y) {
    SET_UNIFORM(2ui, x, y);
}

void opengl::set_uniform(int prog, std::string name, float x, float y, float z) {
    SET_UNIFORM(3f, x, y, z);
}

void opengl::set_uniform(int prog, std::string name, int x, int y, int z) {
    SET_UNIFORM(3i, x, y, z);
}

void opengl::set_uniform(int prog, std::string name, uint x, uint y, uint z) {
    SET_UNIFORM(3ui, x, y, z);
}

void opengl::set_uniform(int prog, std::string name, float x, float y, float z, float w) {
    SET_UNIFORM(4f, x, y, z, w);
}

void opengl::set_uniform(int prog, std::string name, int x, int y, int z, int w) {
    SET_UNIFORM(4i, x, y, z, w);
}

void opengl::set_uniform(int prog, std::string name, uint x, uint y, uint z, uint w) {
    SET_UNIFORM(4ui, x, y, z, w);
}

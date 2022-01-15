#include <sstream>

#include "zz/log.h"

extern "C" {
#include "glad/glad.h"
}

#include "shader.h"

using namespace zz::render;

#define SET_UNIFORM(t, ...) use(); int loc = glGetUniformLocation(id, name.c_str()); glUniform##t(loc, __VA_ARGS__)

shader::shader(std::string &vert, std::string &frag) {
    uint vert_id = glCreateShader(GL_VERTEX_SHADER);
    uint frag_id = glCreateShader(GL_FRAGMENT_SHADER);

    const char *vert_src = vert.c_str();
    int vert_len = vert.length();

    const char *frag_src = frag.c_str();
    int frag_len = frag.length();

    glShaderSource(vert_id, 1, &vert_src, &vert_len);
    glShaderSource(frag_id, 1, &frag_src, &frag_len);

    glCompileShader(vert_id);
    glCompileShader(frag_id);

    int vert_ok, frag_ok;
    glGetShaderiv(vert_id, GL_COMPILE_STATUS, &vert_ok);
    glGetShaderiv(frag_id, GL_COMPILE_STATUS, &frag_ok);

    if (!vert_ok) {
        int len;
        glGetShaderiv(vert_id, GL_INFO_LOG_LENGTH, &len);

        std::string log;
        log.reserve(len);

        glGetShaderInfoLog(vert_id, len, nullptr, log.data());

        ZZ_CORE_ERROR("Vertex shader {} compilation failed: {}", vert_id, log);
        return;
    }

    if (!frag_ok) {
        int len;
        glGetShaderiv(frag_id, GL_INFO_LOG_LENGTH, &len);

        std::string log;
        log.reserve(len);

        glGetShaderInfoLog(frag_id, len, nullptr, log.data());

        ZZ_CORE_ERROR("Fragment shader {} compilation failed: {}", frag_id, log);
        return;
    }

    uint prog_id = glCreateProgram();
    glAttachShader(prog_id, vert_id);
    glAttachShader(prog_id, frag_id);

    glLinkProgram(prog_id);

    int link_ok;
    glGetProgramiv(prog_id, GL_LINK_STATUS, &link_ok);

    if (!link_ok) {
        int len;
        glGetProgramiv(prog_id, GL_INFO_LOG_LENGTH, &len);

        std::string log;
        log.reserve(len);

        glGetProgramInfoLog(prog_id, len, nullptr, log.data());

        ZZ_CORE_ERROR("Shader program {} linkage failed: {}", prog_id, log);
        return;
    }

    id = prog_id;

    glDeleteShader(vert_id);
    glDeleteShader(frag_id);
}

shader::shader(std::istream &vert, std::istream &frag) {
    std::stringstream ss_vert, ss_frag;

    ss_vert << vert.rdbuf();
    ss_frag << frag.rdbuf();

    std::string vert_src = ss_vert.str();
    std::string frag_src = ss_frag.str();

    new (this) shader(vert_src, frag_src);
}

shader::~shader() {
    glDeleteProgram(id);
}

bool shader::is_initialized() { return id >= 0; }

void shader::use() { glUseProgram(id); }

void shader::uniform(std::string name, float x) { SET_UNIFORM(1f, x); }
void shader::uniform(std::string name,   int x) { SET_UNIFORM(1i, x); }
void shader::uniform(std::string name,  uint x) { SET_UNIFORM(1ui, x); }

void shader::uniform(std::string name, float x, float y) { SET_UNIFORM(2f, x, y); }
void shader::uniform(std::string name,   int x,   int y) { SET_UNIFORM(2i, x, y); }
void shader::uniform(std::string name,  uint x,  uint y) { SET_UNIFORM(2ui, x, y); }

void shader::uniform(std::string name, float x, float y, float z) { SET_UNIFORM(3f, x, y, z); }
void shader::uniform(std::string name,   int x,   int y,   int z) { SET_UNIFORM(3i, x, y, z); }
void shader::uniform(std::string name,  uint x,  uint y,  uint z) { SET_UNIFORM(3ui, x, y, z); }

void shader::uniform(std::string name, float x, float y, float z, float w) { SET_UNIFORM(4f, x, y, z, w); }
void shader::uniform(std::string name,   int x,   int y,   int z,   int w) { SET_UNIFORM(4i, x, y, z, w); }
void shader::uniform(std::string name,  uint x,  uint y,  uint z,  uint w) { SET_UNIFORM(4ui, x, y, z, w); }

int shader::get_id() { return id; }

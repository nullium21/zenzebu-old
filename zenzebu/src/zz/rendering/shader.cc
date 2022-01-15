#include <sstream>

#include "zz/log.h"

extern "C" {
#include "glad/glad.h"
}

#include "shader.h"

using namespace zz::render;

#define SET_UNIFORM(t, ...) use(); int loc = glGetUniformLocation(id, name.c_str()); glUniform##t(loc, __VA_ARGS__)

constexpr const char *data_type_to_string(data_type dt) {
    switch (dt) {
        case data_type::float1: return "float";
        case data_type::float2: return "vec2";
        case data_type::float3: return "vec3";
        case data_type::float4: return "vec4";

        case data_type::int1: return "int";
        case data_type::int2: return "ivec2";
        case data_type::int3: return "ivec3";
        case data_type::int4: return "ivec4";

        case data_type::uint1: return "uint";
        case data_type::uint2: return "uvec2";
        case data_type::uint3: return "uvec3";
        case data_type::uint4: return "uvec4";
    }
}

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

shader::shader(
    shader_code &vert, shader_code &frag,
    std::vector<attribute> attrs
): attrs(attrs) {
    std::stringstream ss_vert, ss_frag;

    ss_vert << vert.prefix;
    ss_frag << frag.prefix;

    for (attribute attr : attrs) {
        switch (attr.attr_type) {
            case attribute::vertex_in:
                ss_vert << "in "  << data_type_to_string(attr.type) << " " << attr.name << ";"; break;
            case attribute::fragment_in:
                ss_vert << "out " << data_type_to_string(attr.type) << " " << attr.name << ";";
                ss_frag << "in "  << data_type_to_string(attr.type) << " " << attr.name << ";"; break;
            case attribute::fragment_out:
                ss_frag << "out " << data_type_to_string(attr.type) << " " << attr.name << ";"; break;
            case attribute::uniform:
                ss_vert << "uniform " << data_type_to_string(attr.type) << " " << attr.name << ";";
                ss_frag << "uniform " << data_type_to_string(attr.type) << " " << attr.name << ";"; break;
        }
    }

    ss_vert << "void main() {" << vert.main << "}";
    ss_frag << "void main() {" << frag.main << "}";

    ss_vert << vert.postfix;
    ss_frag << frag.postfix;

    std::string vert_src = ss_vert.str();
    std::string frag_src = ss_frag.str();

    ZZ_CORE_INFO("Generated vertex shader source: {}", vert_src);
    ZZ_CORE_INFO("Generated fragment shader source: {}", frag_src);

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

#include <sstream>
#include <variant>

#include "zz/log.h"
#include "zz/rendering/datatype.h"
#include "zz/rendering/texture.h"

extern "C" {
#include "glad/glad.h"
}

#include "shader.h"

using namespace zz::render;

#define SET_UNIFORM_UNCHECKED(t, ...) \
    /* use(); */ \
    int loc = glGetUniformLocation(id, name.c_str()); \
    glUniform##t(loc, __VA_ARGS__)

#ifdef ZZ_DEBUG
#define SET_UNIFORM(t, t2, ...) \
    if (has_attr(name, datatype::t2)) { \
        SET_UNIFORM_UNCHECKED(t, __VA_ARGS__); \
    } else ZZ_CORE_ERROR("Invalid value given for uniform {}", name)
#else
#define SET_UNIFORM(t, t2, ...) SET_UNIFORM_UNCHECKED(t, __VA_ARGS__)
#endif

void shader::init(std::string &vert, std::string &frag) {
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

shader::shader(
    shader_code &vert, shader_code &frag,
    std::vector<attribute> _attrs) {

    attrs = _attrs;

    std::stringstream ss_vert, ss_frag;

    ss_vert << vert.prefix;
    ss_frag << frag.prefix;

    for (attribute attr : _attrs) {
        switch (attr.attr_type) {
            case attribute::vertex_in:
                ss_vert << "in "  << attr.type.glsl_name << " " << attr.name << ";";
                vertex_stride += attr.type.size;
                break;

            case attribute::fragment_in:
                ss_vert << "out " << attr.type.glsl_name << " " << attr.name << ";";
                ss_frag << "in "  << attr.type.glsl_name << " " << attr.name << ";";
                break;

            case attribute::fragment_out:
                ss_frag << "out " << attr.type.glsl_name << " " << attr.name << ";";
                break;

            case attribute::uniform:
                if (attr.type.glsl_name != datatype::sampler2d.glsl_name)
                    ss_vert << "uniform " << attr.type.glsl_name << " " << attr.name << ";";
                ss_frag << "uniform " << attr.type.glsl_name << " " << attr.name << ";";
                break;
        }
    }

    ss_vert << "void main() {" << vert.main << "}";
    ss_frag << "void main() {" << frag.main << "}";

    ss_vert << vert.postfix;
    ss_frag << frag.postfix;

    std::string vert_src = ss_vert.str();
    std::string frag_src = ss_frag.str();

    ZZ_CORE_INFO("{}", vert_src);
    ZZ_CORE_INFO("{}", frag_src);

    init(vert_src, frag_src);
}

shader::~shader() {
    glDeleteProgram(id);
}

bool shader::is_initialized() { return id >= 0; }

void shader::use() { glUseProgram(id); }

void shader::texture(std::string name, int unit, class texture const &tex) {
    glActiveTexture(GL_TEXTURE0 + unit);
    tex.use();
    SET_UNIFORM(1i, sampler2d, unit);
}

int shader::get_id() { return id; }

void shader::apply_attrs() {
    use();

    int offset = 0;
    for (attribute attr : attrs) {
        if (attr.attr_type != attribute::vertex_in) continue;

        int idx = glGetAttribLocation(id, attr.name.c_str());

        glVertexAttribPointer(idx, attr.type.el_count, attr.type.gl, GL_FALSE, vertex_stride, (void *) offset);

        offset += attr.type.size;
    }
}

std::vector<attribute> &shader::get_attrs() {
    return attrs;
}

bool shader::has_attr(std::string name, datatype::datatype type) {
    for (attribute attr : attrs) {
        if (attr.name == name && attr.type.id == type.id)
            return true;
    }

    return false;
}

void shader::uniforms(const shader_params_holder &params) {
    for (shader_param const &p : params.params) {
        if (has_attr(p.name, p.type)) {
            switch (p.type.id) {
                #include "generated/shader-uniforms-switch.lua.h"
            }
        } else {
            ZZ_CORE_WARN("Unknown shader parameter '{}' ({}), skipping", p.name, p.type.glsl_name);
        }
    }
}

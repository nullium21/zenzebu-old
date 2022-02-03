#include <sstream>
#include <variant>

#include "zz/log.h"
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
    if (has_attr(name, data_type::t2)) { \
        SET_UNIFORM_UNCHECKED(t, __VA_ARGS__); \
    } else ZZ_CORE_ERROR("Invalid value given for uniform {}", name)
#else
#define SET_UNIFORM(t, t2, ...) SET_UNIFORM_UNCHECKED(t, __VA_ARGS__)
#endif

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

        case data_type::sampler2d: return "sampler2D";
    }
}

constexpr int data_type_sizeof(data_type dt) {
    switch (dt) {
        case data_type::float1: return sizeof(float);
        case data_type::float2: return sizeof(float)*2;
        case data_type::float3: return sizeof(float)*3;
        case data_type::float4: return sizeof(float)*4;

        case data_type::int1: return sizeof(int);
        case data_type::int2: return sizeof(int)*2;
        case data_type::int3: return sizeof(int)*3;
        case data_type::int4: return sizeof(int)*4;
        
        case data_type::uint1: return sizeof(uint);
        case data_type::uint2: return sizeof(uint)*2;
        case data_type::uint3: return sizeof(uint)*3;
        case data_type::uint4: return sizeof(uint)*4;

        default: return 0;
        
    }
}

constexpr int dt_count(data_type dt) {
    switch (dt) {
        case data_type::float1: case data_type::int1: case data_type::uint1:
            return 1;

        case data_type::float2: case data_type::int2: case data_type::uint2:
            return 2;

        case data_type::float3: case data_type::int3: case data_type::uint3:
            return 3;

        case data_type::float4: case data_type::int4: case data_type::uint4:
            return 4;

        default: return 0;
    }
}

constexpr int dt_item_type(data_type dt) {
    switch (dt) {
        case data_type::float1: case data_type::float2: case data_type::float3: case data_type::float4:
            return GL_FLOAT;

        case data_type::int1: case data_type::int2: case data_type::int3: case data_type::int4:
            return GL_INT;

        case data_type::uint1: case data_type::uint2: case data_type::uint3: case data_type::uint4:
            return GL_UNSIGNED_BYTE;

        default: return 0;
    }
}

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
                ss_vert << "in "  << data_type_to_string(attr.type) << " " << attr.name << ";";
                vertex_stride += data_type_sizeof(attr.type);
                break;

            case attribute::fragment_in:
                ss_vert << "out " << data_type_to_string(attr.type) << " " << attr.name << ";";
                ss_frag << "in "  << data_type_to_string(attr.type) << " " << attr.name << ";";
                break;

            case attribute::fragment_out:
                ss_frag << "out " << data_type_to_string(attr.type) << " " << attr.name << ";";
                break;

            case attribute::uniform:
                if (attr.type != data_type::sampler2d)
                    ss_vert << "uniform " << data_type_to_string(attr.type) << " " << attr.name << ";";
                ss_frag << "uniform " << data_type_to_string(attr.type) << " " << attr.name << ";";
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

        glVertexAttribPointer(idx, dt_count(attr.type), dt_item_type(attr.type), GL_FALSE, vertex_stride, (void *) offset);

        offset += data_type_sizeof(attr.type);
    }
}

std::vector<attribute> &shader::get_attrs() {
    return attrs;
}

bool shader::has_attr(std::string name, data_type type) {
    for (attribute attr : attrs) {
        if (attr.name == name && attr.type == type)
            return true;
    }

    return false;
}

void shader::uniforms(const shader_params_holder &params) {
    for (shader_param const &p : params.params) {
        if (has_attr(p.name, p.type)) {
            switch (p.type) {
                case data_type::float1: uniform(p.name, std::get<glm::vec<1, float>>(p.value).x); break;
                case data_type::float2: uniform(p.name, std::get<glm::vec<2, float>>(p.value).x, std::get<glm::vec<2, float>>(p.value).y); break;
                case data_type::float3: uniform(p.name, std::get<glm::vec<3, float>>(p.value).x, std::get<glm::vec<3, float>>(p.value).y, std::get<glm::vec<3, float>>(p.value).z); break;
                case data_type::float4: uniform(p.name, std::get<glm::vec<4, float>>(p.value).x, std::get<glm::vec<4, float>>(p.value).y, std::get<glm::vec<4, float>>(p.value).z, std::get<glm::vec<4, float>>(p.value).w); break;
                case data_type::int1  : uniform(p.name, std::get<glm::vec<1,   int>>(p.value).x); break;
                case data_type::int2  : uniform(p.name, std::get<glm::vec<2,   int>>(p.value).x, std::get<glm::vec<2,   int>>(p.value).y); break;
                case data_type::int3  : uniform(p.name, std::get<glm::vec<3,   int>>(p.value).x, std::get<glm::vec<3,   int>>(p.value).y, std::get<glm::vec<3,   int>>(p.value).z); break;
                case data_type::int4  : uniform(p.name, std::get<glm::vec<4,   int>>(p.value).x, std::get<glm::vec<4,   int>>(p.value).y, std::get<glm::vec<4,   int>>(p.value).z); break;
                case data_type::uint1 : uniform(p.name, std::get<glm::vec<1,  uint>>(p.value).x); break;
                case data_type::uint2 : uniform(p.name, std::get<glm::vec<2,  uint>>(p.value).x, std::get<glm::vec<2,  uint>>(p.value).y); break;
                case data_type::uint3 : uniform(p.name, std::get<glm::vec<3,  uint>>(p.value).x, std::get<glm::vec<3,  uint>>(p.value).y, std::get<glm::vec<3,  uint>>(p.value).z); break;
                case data_type::uint4 : uniform(p.name, std::get<glm::vec<4,  uint>>(p.value).x, std::get<glm::vec<4,  uint>>(p.value).y, std::get<glm::vec<4,  uint>>(p.value).z, std::get<glm::vec<4,  uint>>(p.value).w); break;

                case data_type::sampler2d: texture(p.name, std::get<class texture *>(p.value)->unit, *std::get<class texture *>(p.value)); break;
            }
        } else {
            ZZ_CORE_WARN("Unknown shader parameter '{}' ({}), skipping", p.name, data_type_to_string(p.type));
        }
    }
}

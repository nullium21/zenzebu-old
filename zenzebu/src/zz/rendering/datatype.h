#pragma once

#include <cstddef>
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace zz::render::datatype {
	using uint = unsigned int;

	struct datatype {
        size_t size, gl, el_count, id;
        char *glsl_name;

        bool operator==(datatype other) { return id == other.id; }
        bool operator!=(datatype other) { return id != other.id; }
    };

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wwrite-strings"
    const datatype sampler2d = { .size = 0, .gl = 0, .el_count = 0, .id = 0, .glsl_name = "sampler2D" };

    #include "generated/gl-datatypes.lua.h"
    #pragma GCC diagnostic pop
}

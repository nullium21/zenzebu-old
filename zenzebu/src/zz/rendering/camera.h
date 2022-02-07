#pragma once

#include "glm/mat4x4.hpp"

namespace zz::render {
	struct camera {
		glm::mat4 projection;
		glm::vec3 pos, target;
	};
}
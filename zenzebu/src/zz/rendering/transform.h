#pragma once

#include "glm/matrix.hpp"
#include "glm/vec3.hpp"

#include "glm/gtc/matrix_transform.hpp"

namespace zz::render {
	struct transform {
		glm::mat4 matrix;

		void translate(glm::vec3 offset) { matrix = glm::translate(matrix, offset); }
		void scale    (glm::vec3  scale) { matrix = glm::scale    (matrix,  scale); }

		void rotate_deg(glm::vec3 axis, float value) { matrix = glm::rotate(matrix, glm::radians(value), axis); }
		void rotate_rad(glm::vec3 axis, float value) { matrix = glm::rotate(matrix,              value , axis); }
	};
};
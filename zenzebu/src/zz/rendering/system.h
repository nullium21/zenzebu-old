#pragma once

#define GLFW_INCLUDE_NONE

#include <glad/glad.h>

#include "zz/ecs.h"
#include "zz/window.h"
#include "zz/rendering/render_target.h"
#include "zz/rendering/meshbuffer.h"
#include "zz/rendering/shader.h"
#include "zz/rendering/transform.h"
#include "zz/rendering/camera.h"
#include "zz/rendering/gl.h"
#include "glm/vec3.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace zz::render {
	class rendering {
	public:
		static bool init() {
			if (windowing::init()) {
				opengl::load();

				glEnable(GL_DEPTH_TEST);

				return true;
			}

			return false;
		}
		static bool deinit() {
			return windowing::deinit();
		}

		// Vt = vertex data type
		template<class Vt = glm::vec3>
		static bool update() {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			auto view = ecs::entt()->view<const window_render_target, const component::children>();

			view.each([](entity e, const window_render_target &rt, const component::children children) {
				if (rt.window == nullptr) return;
				rt.window->use();

				auto cam = ecs::entt()->try_get<camera>(e);
				if (cam == nullptr) return;

				auto cdirinv = glm::normalize(cam->pos - cam->target);
				auto cright  = glm::normalize(glm::cross(glm::vec3(0, 1, 0), cdirinv));
				auto cup     = glm::cross(cdirinv, cright);
				auto cview   = glm::lookAt(cam->pos, glm::vec3(0, 0, 0), cup);

				for (entity child : children.children) {
					auto buf = ecs::entt()->try_get<meshbuffer<Vt>>(child);
					if (buf == nullptr) continue;

					auto shd = ecs::entt()->try_get<shader>(child);
					if (shd == nullptr) continue;

					shd->use();

					auto params = ecs::entt()->try_get<shader_params_holder>(child);
					if (params != nullptr) shd->uniforms(*params);

					auto tfm = ecs::entt()->try_get<transform>(child);
					if (tfm != nullptr) shd->uniform("transform", tfm->matrix);

					if (cam != nullptr) {
						shd->uniform("projection", cam->projection);
						shd->uniform("view", cview);
					}

					buf->draw();
				}
			});

			return windowing::update();
		}
	};
}

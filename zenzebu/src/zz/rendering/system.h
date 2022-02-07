#pragma once

#define GLFW_INCLUDE_NONE

#include "zz/ecs.h"
#include "zz/window.h"
#include "zz/rendering/render_target.h"
#include "zz/rendering/meshbuffer.h"
#include "zz/rendering/shader.h"
#include "zz/rendering/transform.h"
#include "glm/vec3.hpp"

namespace zz::render {
	class rendering {
	public:
		static bool init() {
			return windowing::init();
		}
		static bool deinit() {
			return windowing::deinit();
		}

		// Vt = vertex data type
		template<class Vt = glm::vec3>
		static bool update() {
			auto view = ecs::entt()->view<const window_render_target, const component::children>();

			view.each([](entity e, const window_render_target &rt, const component::children children) {
				if (rt.window == nullptr) return;
				rt.window->use();

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

					buf->draw();
				}
			});

			return windowing::update();
		}
	};
}

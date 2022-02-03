#pragma once

#include "zz/window.h"
#include <memory>

namespace zz::render {
	struct window_render_target {
	public:
		zz::window *window;
	};
}
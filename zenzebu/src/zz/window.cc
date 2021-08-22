#include "GLFW/glfw3.h"
#include "zz/ecs.h"
#include "zz/log.h"
#include "zz/window.h"

using namespace zz::ecs;
using namespace zz::log;

using zz::window;
using zz::windowing;

window::~window() {
    if (initialized) glfwDestroyWindow(wnd);
}

void window::init() {
    wnd = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    if (wnd == nullptr) {
        ZZ_CORE_ERROR("failed to initialize window '{0}' ({1}x{2})", title, width, height);
    }

    glfwMakeContextCurrent(wnd);
    glfwSetWindowUserPointer(wnd, this);

    if (vsync)
        glfwSwapInterval(1);

    ZZ_CORE_INFO("window '{0}' initialized", title);
}

bool windowing::init() {
    if (!initialized) {
        int success = glfwInit();

        if (!success) {
            ZZ_CORE_ERROR("failed to initialize GLFW");
            return false;
        }
        
        initialized = true;
    }

    auto view = zz::ecs::ecs::entt()->view<window>();

    view.each([](window &wnd) {
        wnd.init();
    });

    return true;
}

bool windowing::deinit() {
    if (initialized) {
        ecs::ecs::entt()->clear<window>(); // lets hope this'll invoke the destructor

        initialized = false;
    }

    return true;
}

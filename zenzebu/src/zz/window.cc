#ifndef ZZ_USEPCH
#include "zz/log.h"
#endif

#include "GLFW/glfw3.h"
#include "zz/ecs.h"
#include "zz/window.h"

using namespace zz;

window::window(string t, unsigned int w, unsigned int h, bool vs)
    : title(t), width(w), height(h), vsync(vs) {}

window::~window() {
    close();
}

void window::init() {
    wnd = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    if (wnd == nullptr) {
        ZZ_CORE_ERROR("failed to initialize window '{0}' ({1}x{2})", title, width, height);
    }

    glfwMakeContextCurrent(wnd);
    glfwSetWindowUserPointer(wnd, this);

    glfwSwapInterval(vsync ? 1 : 0);

    glfwSetWindowSizeCallback(wnd, glfw_on_resize);

    ZZ_CORE_INFO("window '{0}' initialized", title);
}

void window::update() {
    glfwMakeContextCurrent(wnd);

    if (old_title != title)
        glfwSetWindowTitle(wnd, title.c_str());

    if (width != old_width || height != old_height)
        glfwSetWindowSize(wnd, width, height);

    if (vsync != old_vsync)
        glfwSwapInterval(vsync ? 1 : 0);

    old_title = title;
    old_width = width; old_height = height;
    old_vsync = vsync;

    glfwSwapBuffers(wnd);
}

void window::close() {
    if (wnd != nullptr) {
        glfwDestroyWindow(wnd);
        wnd = nullptr;
    }
}

void window::set_resize_callback(resize_callback cb) {
    this->on_resize = cb;
}

void window::glfw_on_resize(GLFWwindow *wnd, int w, int h) {
    window *self = (window *) glfwGetWindowUserPointer(wnd);

    self->width = w;
    self->height = h;

    self->on_resize(self, w, h);
}

bool window::should_close() {
    return glfwWindowShouldClose(wnd);
}

bool windowing::init() {
    int success = glfwInit();

    if (!success) {
        ZZ_CORE_ERROR("failed to initialize GLFW");
        return false;
    }
        
    glfwSetErrorCallback(on_glfw_error);

    auto view = ecs::entt()->view<window>();

    view.each([](window &wnd) {
        wnd.init();
    });

    return true;
}

bool windowing::update() {
    auto view = ecs::entt()->view<window>();

    bool any_updated = false;

    view.each([&any_updated](entity e, window &wnd) {
        if (wnd.should_close()) {
            ZZ_CORE_INFO("closing window '{0}'", wnd.title);
            wnd.close();
            ecs::entt()->remove<window>(e);
        } else {
            wnd.update();
            any_updated = true;
        }
    });

    glfwPollEvents();

    return any_updated;
}

bool windowing::deinit() {
    ecs::entt()->clear<window>(); // lets hope this'll invoke the destructor
    
    glfwTerminate();

    return true;
}

void windowing::on_glfw_error(int err, const char *msg) {
    ZZ_CORE_ERROR("GLFW error {0}: {1}", err, msg);
}

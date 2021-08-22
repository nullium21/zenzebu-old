#include "GLFW/glfw3.h"
#include "zz/ecs.h"
#include "zz/log.h"
#include "zz/window.h"

using namespace zz;

window::window(string t, unsigned int w, unsigned int h, bool vs)
    : title(t), width(w), height(h), vsync(vs) {}

window::~window() {
    if (wnd != nullptr) glfwDestroyWindow(wnd);
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

void window::set_resize_callback(resize_callback cb) {
    this->on_resize = cb;
}

void window::glfw_on_resize(GLFWwindow *wnd, int w, int h) {
    window *self = (window *) glfwGetWindowUserPointer(wnd);

    self->width = w;
    self->height = h;

    self->on_resize(self, w, h);
}

bool windowing::init() {
    // if (!initialized) {
        int success = glfwInit();

        if (!success) {
            ZZ_CORE_ERROR("failed to initialize GLFW");
            return false;
        }
        
        // initialized = true;
    // }

    auto view = ecs::entt()->view<window>();

    view.each([](window &wnd) {
        wnd.init();
    });

    return true;
}

bool windowing::update() {
    // if (!initialized && !init()) // init() will invoke only if !initialized
    //     return false;

    auto view = ecs::entt()->view<window>();
    view.each([](window &wnd) {
        wnd.update();
    });

    return true;
}

bool windowing::deinit() {
    // if (initialized) {
        ecs::entt()->clear<window>(); // lets hope this'll invoke the destructor

        glfwTerminate();

        // initialized = false;
    // }

    return true;
}

#include "rendering.h"
#include "GLFW/glfw3.h"
#include "window.h"
#include "ecs.h"

using namespace zz;

renderer::renderer() {

}

renderer::~renderer() {

}

void renderer::render(component::mesh *msh, GLFWwindow *wnd) {

}

void renderer::init(const entity e, GLFWwindow *wnd) {
    glfwMakeContextCurrent(wnd);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    window *w = static_cast<window *>(glfwGetWindowUserPointer(wnd));

    auto msh = ecs::entt()->try_get<component::mesh>(e);
    if (msh != nullptr && msh->vbo == -1 && msh->should_create_buffers_on_init()) {
        glGenBuffers(1, &msh->vbo);
    }
}

void renderer::update(const entity e, GLFWwindow *wnd) {
    window *w = static_cast<window *>(glfwGetWindowUserPointer(wnd));

    std::shared_ptr<entt::registry> reg = ecs::entt();

    auto children = reg->try_get<component::children>(e);
    if (children != nullptr) {
        for (entity child : children->children) {
            if (reg->try_get<component::parent>(child) == nullptr) continue;

            auto mesh = reg->try_get<component::mesh>(child);

            if (mesh != nullptr) render(child, mesh, wnd);
        }
    }
}

bool rendering::init() {
    auto view = ecs::entt()->view<renderer, const window>();
    view.each([](const auto entity, renderer &r, const window &w) {
        r.init(entity, w.wnd);
    });

    return true;
}

bool rendering::update() {
    auto view = ecs::entt()->view<renderer, const window>();
    view.each([](const auto entity, renderer &r, const window &w) {
        r.update(entity, w.wnd);
    });

    return true;
}

bool rendering::deinit() {
    return true;
}

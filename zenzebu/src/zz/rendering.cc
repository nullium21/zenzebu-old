#include "rendering.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "window.h"
#include "ecs.h"
#include "log.h"

using namespace zz;

renderer::renderer() {

}

renderer::~renderer() {

}

void renderer::render(const entity e, component::mesh *msh, GLFWwindow *wnd) {
    if (msh->vbo == -1) {
        glGenBuffers(1, &msh->vbo);
    }

    GLenum draw_type;
    switch (msh->type) {
        case component::mesh::draw_dynamic: draw_type = GL_DYNAMIC_DRAW;
        case component::mesh::draw_static : draw_type = GL_STATIC_DRAW;

        default: {
            log::core_logger()->error("unsupported draw type for mesh: {}", msh->type);
            return;
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, msh->vbo);
    if (msh->type != component::mesh::draw_static) { // in static meshes, data is already uploaded?
        float *verts = msh->vert_buffer_all();
        size_t verts_size = msh->indices.size() * 3 * sizeof(float);

        glBufferData(GL_ARRAY_BUFFER, verts_size, verts, draw_type);
    }
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

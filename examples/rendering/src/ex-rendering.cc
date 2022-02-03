extern "C" {
#include <glad/glad.h>
#include "GLFW/glfw3.h"
}
#include "zz.h"
#include "glm/glm.hpp"

using namespace zz;
using namespace zz::app;
using namespace zz::render;

class rendering_app: public application {
    void run() override {
        log::init();
        ecs::init();

        ZZ_INFO("starting rendering example");

        auto entt = ecs::entt();

        auto window_ent = entt->create();
        auto &wnd = entt->emplace<window>(window_ent, "rendering example", 1024, 768, true);
        auto &target = entt->emplace<window_render_target>(window_ent, &wnd);
        auto &children = entt->emplace<zz::component::children>(window_ent);

        auto mesh_ent = entt->create();
        entt->emplace<zz::component::parent>(mesh_ent, window_ent);
        children.children.push_back(mesh_ent);

        ZZ_INFO("Entity ID: {}", window_ent);

        rendering::init();
        wnd.use();

        opengl::load();

        char *glver = (char *) glGetString(GL_VERSION);
        ZZ_INFO("GL version: {}", glver);

        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, GLenum sev, GLsizei len, const GLchar *msg, const void *param) {
            ZZ_WARN("GL callback (T={}, S={}): {}", type, sev, msg);
        }, 0);

        shader_code vert = {
            .prefix  = "#version 330 core\n",
            .main    = "gl_Position = vec4(.5 * pos.x, .5 * pos.y, pos.z, 1.0); tex_f = vec2(tex_v.x, tex_v.y);",
            .postfix = ""
        };

        shader_code frag = {
            .prefix  = "#version 330 core\n",
            .main    = "fgc = texture(tex, tex_f);",
            .postfix = ""
        };

        std::vector<render::attribute> sh_attrs {
            { "pos"  , data_type::float3, attribute::vertex_in },     // vertex   input : vert position
            { "t"    , data_type::int1  , attribute::uniform },       // uniform        : time
            { "fgc"  , data_type::float4, attribute::fragment_out },  // fragment output: color
            { "tex_f", data_type::float2, attribute::fragment_in },   // fragment input : texture coords
            { "tex_v", data_type::float2, attribute::vertex_in },     // vertex   input : texture coords
            { "tex"  , data_type::sampler2d, attribute::uniform },    // uniform        : texture
        };

        auto &sh = entt->emplace<shader>(mesh_ent, vert, frag, sh_attrs);

        std::vector<textured_vert> verts {
            { {  1,  1, 0 }, { 1, 1 } },
            { {  1, -1, 0 }, { 1, 0 } },
            { { -1, -1, 0 }, { 0, 0 } },
            { { -1,  1, 0 }, { 0, 1 } }
        };

        std::vector<int> idxs {
            0, 1, 3,
            1, 2, 3
        };

        auto &msh = entt->emplace<meshbuffer<textured_vert>>(mesh_ent, verts, idxs, draw_type::static_draw);

        texture tex("examples/rendering/res/wall.jpg");

        ZZ_INFO("before sparams creation");

        std::vector<shader_param> sparams {
            { "t", data_type::int1, glm::vec<1, int>(0) },
            { "tex", data_type::sampler2d, &tex }
        };

        ZZ_INFO("after sparams creation");

        auto &params = entt->emplace<shader_params_holder>(mesh_ent, sparams);

        wnd.use();
        sh.apply_attrs();

        glEnableVertexAttribArray(0);                       // enable attribute #0 for shader
        glEnableVertexAttribArray(1);

        int t = 0, dt = 1;

        do {
            // ZZ_INFO("{} NE={} IE={} IV={} IO={} IF={} OM={}", glGetError(),
            //     GL_NO_ERROR, GL_INVALID_ENUM, GL_INVALID_VALUE, GL_INVALID_OPERATION, GL_INVALID_FRAMEBUFFER_OPERATION, GL_OUT_OF_MEMORY);

            t += dt;
            if (t >= 1024) dt = -1;
            if (t <=    0) dt = +1;
            if (dt <    0) dt--;
            if (dt >=   0) dt++;
        } while (rendering::update<textured_vert>());

        wnd.use();
        glDisableVertexAttribArray(0);                      // disable the shader attribute so that it doesn't cause troubles later
        glDisableVertexAttribArray(1);

        rendering::deinit();
    }
};

namespace zz::app {
    application *create_app() {
        return new rendering_app();
    }
}
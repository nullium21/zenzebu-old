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

        auto ent = entt->create();
        auto &wnd = entt->emplace<window>(ent, "rendering example", 1024, 768, true);

        ZZ_INFO("Entity ID: {}", ent);

        windowing::init();
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

        shader sh(vert, frag, sh_attrs);

        meshbuffer<textured_vert> msh({
            { {  1,  1, 0 }, { 1, 1 } },
            { {  1, -1, 0 }, { 1, 0 } },
            { { -1, -1, 0 }, { 0, 0 } },
            { { -1,  1, 0 }, { 0, 1 } }
        }, {
            0, 1, 3,
            1, 2, 3
        }, draw_type::static_draw);

        texture tex("examples/rendering/res/wall.jpg");

        wnd.use();
        sh.apply_attrs();

        glEnableVertexAttribArray(0);                       // enable attribute #0 for shader
        glEnableVertexAttribArray(1);

        int t = 0, dt = 1;

        do {
            ZZ_INFO("{} NE={} IE={} IV={} IO={} IF={} OM={}", glGetError(),
                GL_NO_ERROR, GL_INVALID_ENUM, GL_INVALID_VALUE, GL_INVALID_OPERATION, GL_INVALID_FRAMEBUFFER_OPERATION, GL_OUT_OF_MEMORY);

            wnd.use();

            sh.use();
            sh.uniform("t", t);

            sh.texture("tex", 0, tex);

            msh.use();
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            t += dt;
            if (t >= 1024) dt = -1;
            if (t <=    0) dt = +1;
            if (dt <    0) dt--;
            if (dt >=   0) dt++;
        } while (windowing::update());

        wnd.use();
        glDisableVertexAttribArray(0);                      // disable the shader attribute so that it doesn't cause troubles later
        glDisableVertexAttribArray(1);

        windowing::deinit();
    }
};

namespace zz::app {
    application *create_app() {
        return new rendering_app();
    }
}
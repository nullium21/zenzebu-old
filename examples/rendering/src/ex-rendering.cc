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

        shader_code vert = {
            .prefix  = "#version 330\n",
            .main    = "gl_Position = vec4(.5 * pos.x, .5 * pos.y, pos.z, 1.0);",
            .postfix = ""
        };

        shader_code frag = {
            .prefix  = "#version 330\n",
            .main    = "fgc = vec4(t / 1024.0, 0.0, 0.0, 1.0);",
            .postfix = ""
        };

        std::vector<render::attribute> sh_attrs {
            { "pos", data_type::float3, attribute::vertex_in },
            { "t"  , data_type::int1  , attribute::uniform },
            { "fgc", data_type::float4, attribute::fragment_out },
        };

        shader sh(vert, frag, sh_attrs);

        meshbuffer msh({
            {  1,  1, 0 },
            {  1, -1, 0 },
            { -1, -1, 0 },
            { -1,  1, 0 }
        }, {
            0, 1, 3,
            1, 2, 3
        }, draw_type::static_draw);

        wnd.use();
        sh.apply_attrs();

        glEnableVertexAttribArray(0);                       // enable attribute #0 for shader

        int t = 0, dt = 1;

        do {
            wnd.use();

            sh.uniform("t", t);
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

        windowing::deinit();
    }
};

namespace zz::app {
    application *create_app() {
        return new rendering_app();
    }
}
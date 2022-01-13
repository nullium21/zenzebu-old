extern "C" {
#include <glad/glad.h>
}
#include "zz.h"
#include "glm/glm.hpp"

using namespace zz;
using namespace zz::app;

class rendering_app: public application {
    void run() override {
        log::init();
        ecs::init();

        ZZ_INFO("starting rendering example");

        auto entt = ecs::entt();

        auto ent = entt->create();
        auto wnd = entt->emplace<window>(ent, "rendering example", 1024, 768, true);

        windowing::init();

        gladLoadGL();

        wnd.use();

        float vert_data[3] = { 0, 0, 0 };                   // data for vertices: 3 floats for each (X Y Z)

        GLuint vbo;
        glGenBuffers(1, &vbo);                              // generate a buffer for the data

        glBindBuffer(GL_ARRAY_BUFFER, vbo);                 // use the buffer as a vertex buffer (vertex array)

        glBufferData(GL_ARRAY_BUFFER, sizeof(vert_data),    // copy the data:
            vert_data, GL_STATIC_DRAW);                     // - data type (vertices)
                                                            // - data size in bytes
                                                            // - the data itself
                                                            // - data type (will it be changed?)

        glEnableVertexAttribArray(0);                       // enable attribute #0 for shader

        // glBindBuffer(GL_ARRAY_BUFFER, vbo);                 // why do i need to do this again???

        glVertexAttribPointer(                              // set how it'll be used in the shader:
            0, 3,                                           // - index of the array (0), number of entries in every piece of data (3 - X Y Z),
            GL_FLOAT,                                       // - type of the elements (float),
            GL_FALSE,                                       // - should the data be normalized?
            sizeof(float)*3, 0);                            // - gap between elements, offset

        while (windowing::update()) {
            wnd.use();
            glDrawArrays(GL_POINTS, 0, 1);                  // draw the data in the arrays: starting from 0th, 1 element in total
        }

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
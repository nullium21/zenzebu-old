extern "C" {
#include <glad/glad.h>
#include "GLFW/glfw3.h"
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
        auto &wnd = entt->emplace<window>(ent, "rendering example", 1024, 768, true);

        ZZ_INFO("Entity ID: {}", ent);

        windowing::init();
        wnd.use();

        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

        char *glver = (char *) glGetString(GL_VERSION);
        ZZ_INFO("GL version: {}", glver);

        GLuint shprog, shvert, shfrag;
        std::string vert_src =
            "#version 330\n"
            "layout (location = 0) in vec3 pos;\n"
            "void main() {\n"
            "  gl_Position = vec4(.5 * pos.x, .5 * pos.y, pos.z, 1.0);\n"
            "}";
        int vert_len = vert_src.length();
        const char *vert_src_c = vert_src.c_str();

        std::string frag_src = 
            "#version 330\n"
            "out vec4 frag_color;\n"
            "void main() {\n"
            "  frag_color = vec4(1.0, 0.0, 0.0, 1.0);\n"
            "}";
        int frag_len = frag_src.length();
        const char *frag_src_c = frag_src.c_str();

        shprog = glCreateProgram();
        shvert = glCreateShader(GL_VERTEX_SHADER);
        shfrag = glCreateShader(GL_FRAGMENT_SHADER);

        glShaderSource(shvert, 1, &vert_src_c, &vert_len);
        glShaderSource(shfrag, 1, &frag_src_c, &frag_len);

        int isok, err;
        char info_log[1024];

        glCompileShader(shvert);
        glGetShaderiv(shvert, GL_COMPILE_STATUS, &isok);
        if (isok) glAttachShader(shprog, shvert);
        else {
            glGetShaderInfoLog(shvert, sizeof(info_log), nullptr, info_log);
            ZZ_ERROR("Vertex shader compilation failed: {}", info_log);
            // fprintf(stderr, "%s", info_log);
        } 

        glCompileShader(shfrag);
        glGetShaderiv(shfrag, GL_COMPILE_STATUS, &isok);
        if (isok) glAttachShader(shprog, shfrag);
        else {
            glGetShaderInfoLog(shfrag, sizeof(info_log), nullptr, info_log);
            ZZ_ERROR("Fragment shader compilation failed: {}", info_log);
            // fprintf(stderr, "%s", info_log);
        }

        glLinkProgram(shprog);
        glGetProgramiv(shprog, GL_LINK_STATUS, &isok);
        if (!isok) {
            glGetProgramInfoLog(shprog, sizeof(info_log), nullptr, info_log);
            ZZ_ERROR("Shader program linkage failed: {}", info_log);
            // fprintf(stderr, "%s", info_log);
        }

        float vert_data[] = {                              // data for vertices: 3 floats for each (X Y Z)
             1.,  1., 0.,
             1., -1., 0.,
            -1., -1., 0.,
            -1.,  1., 0.,
        };

        unsigned int indices[] = {
            0, 1, 3,
            1, 2, 3,
        };

        GLuint vbo, vao, ebo;
        glGenBuffers(1, &vbo);                              // generate a buffer for the data
        glGenBuffers(1, &ebo);
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);                 // use the buffer as a vertex buffer (vertex array)

        glBufferData(GL_ARRAY_BUFFER, sizeof(vert_data),    // copy the data:
            vert_data, GL_STATIC_DRAW);                     // - data type (vertices)
                                                            // - data size in bytes
                                                            // - the data itself
                                                            // - data type (will it be changed?)

        // glBindBuffer(GL_ARRAY_BUFFER, vbo);                 // why do i need to do this again???

        glVertexAttribPointer(                              // set how it'll be used in the shader:
            0, 3,                                           // - index of the array (0), number of entries in every piece of data (3 - X Y Z),
            GL_FLOAT,                                       // - type of the elements (float),
            GL_FALSE,                                       // - should the data be normalized?
            sizeof(float)*3, 0);                            // - gap between elements, offset

        glEnableVertexAttribArray(0);                       // enable attribute #0 for shader

        do {
            wnd.use();
            glUseProgram(shprog);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
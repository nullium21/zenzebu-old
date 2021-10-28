#pragma once

#define GLFW_INCLUDE_NONE
extern "C" {
#include "GLFW/glfw3.h"
}

#include "ecs.h"
#include "mesh.h"

namespace zz {
    class rendering {
    public:
        static bool init();
        static bool update();
        static bool deinit();
    };

    class renderer {
    public:
        renderer();
        ~renderer();

        void init(const entity, GLFWwindow *);
        void update(const entity, GLFWwindow *);
        void render(const entity, component::mesh *, GLFWwindow *);
    private:
        bool initialized = false;
    };
}
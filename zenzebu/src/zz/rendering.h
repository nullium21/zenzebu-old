#pragma once

#include "zz/mesh.h"
extern "C" {
#include "GLFW/glfw3.h"
}

#include "ecs.h"

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
        void render(component::mesh *, GLFWwindow *);
    private:
        bool initialized = false;
    };
}
#pragma once

extern "C" {
#include "GLFW/glfw3.h"
}

using std::string;

namespace zz {
    class window {
    public:
        ~window();
        void init();

        string title;
        unsigned int width;
        unsigned int height;
        bool vsync;
    private:
        GLFWwindow *wnd = nullptr;
        bool initialized = false;

        friend class windowing;
    };

    class windowing {
    public:
        static bool init();
        static bool deinit();
    private:
        static bool initialized;

        friend class window;
    };

    // void init_windows();
}

#pragma once

extern "C" {
#include "GLFW/glfw3.h"
}

using std::string;

namespace zz {
    class window {
    public:
        window(string, unsigned int, unsigned int, bool);
        ~window();

        void init();
        void update();

        string title;
        unsigned int width;
        unsigned int height;
        bool vsync;
    private:
        GLFWwindow *wnd = nullptr;

        string old_title;
        unsigned int old_width;
        unsigned int old_height;
        bool old_vsync;
    };

    class windowing {
    public:
        static bool init();

        static bool update();

        static bool deinit();

        static bool initialized;
    };

    // void init_windows();
}

#pragma once

#ifndef ZZ_USEPCH
#include <string>
#include <functional>
#endif

extern "C" {
#include "GLFW/glfw3.h"
}

using std::string;

namespace zz {
    class window {
    public:
        using resize_callback = std::function<void(window *, unsigned int, unsigned int)>;

        window(string, unsigned int, unsigned int, bool);
        ~window();

        void init();
        void update();
        void close();

        string title;
        unsigned int width;
        unsigned int height;
        bool vsync;

        void set_resize_callback(resize_callback);

        bool should_close();
    private:
        GLFWwindow *wnd = nullptr;

        string old_title;
        unsigned int old_width;
        unsigned int old_height;
        bool old_vsync;

        resize_callback on_resize;

        static void glfw_on_resize(GLFWwindow *, int, int);
    };

    class windowing {
    public:
        static bool init();

        static bool update();

        static bool deinit();

    private:
        static void on_glfw_error(int, const char *);
    };

}

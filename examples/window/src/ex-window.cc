#include <string>

#include "zz/app.h"
#include "zz/log.h"
#include "zz/ecs.h"
#include "zz/window.h"

using namespace zz;
using namespace zz::app;

class wnd_app: public application {
public:
    void run() override {
        log::init();
        ecs::init();

        std::string title = "test window";
        unsigned int width = 1024;
        unsigned int height = 768;
        bool vsync = true;

        auto ent = ecs::entt()->create();
        ecs::entt()->emplace<window>(ent, title, width, height, vsync);

        windowing::init();

        windowing::update();

        windowing::deinit();
    }
};

namespace zz::app {
    application *create_app() {
        return new wnd_app();
    }
}
#include <sstream>
#include <string>

#include "zz/app.h"
#include "zz/log.h"
#include "zz/ecs.h"
#include "zz/script.h"

using namespace zz::log;
using namespace zz::scripting;
using namespace zz::app;
using namespace zz::ecs;

class ns_app: public application {
public:
    void run() override {
        log::init();
        ecs::init();

        ZZ_INFO("starting native scripting application");

        for (auto i = 0u; i < 10u; i++) {
            const auto e = ecs::entt()->create();

            std::stringstream sname;
            sname << "native script for entity " << i;
            std::string name = sname.str();

            std::function<void()> on_start = [name]() {
                ZZ_INFO("script '{0}' on_start", name);
            };

            std::function<void()> on_update = [name]() {
                ZZ_INFO("script '{0}' on_update", name);
            };

            ecs::entt()->emplace<native_script>(e, name, on_start, on_update);
        }

        for (auto i = 0u; i < 10u; i++) {
            zz::scripting::update();
        }
    }
};

namespace zz::app {
    application *create_app() {
        return new ns_app();
    }
}
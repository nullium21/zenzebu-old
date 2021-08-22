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
            sname << "entity " << i;
            std::string name = sname.str();

            native::script_callback on_start = [name](auto _) {
                ZZ_INFO("script '{0}' on_start", name);
            };

            native::script_callback on_update = [name](auto _) {
                ZZ_INFO("script '{0}' on_update", name);
            };

            if (i % 2 == 0)
                ecs::entt()->emplace<native::on_init_script>(e, on_start);

            ecs::entt()->emplace<native::on_update_script>(e, on_update);
        }

        update_scripts(update_stage::us_oninit);

        for (auto i = 0u; i < 10u; i++) {
            update_scripts(update_stage::us_onupdate);
        }
    }
};

namespace zz::app {
    application *create_app() {
        return new ns_app();
    }
}
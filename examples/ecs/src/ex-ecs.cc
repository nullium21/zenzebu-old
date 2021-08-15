#include "zz/app.h"
#include "zz/log.h"
#include "zz/ecs.h"

using namespace zz::log;
using namespace zz::app;
using namespace zz::ecs;

struct dbg_out {
    std::string text;
    uint32_t id;
};

class ecs_app: public application {
private:
    void update_ecs() {
        auto view = ecs::entt()->view<const dbg_out>();

        view.each([](const auto &out) {
            ZZ_INFO("{0} {1}", out.id, out.text);
        });
    }
public:
    void run() override {
        log::init();
        ecs::init();

        ZZ_INFO("starting ECS application");

        for (auto i = 0u; i < 10u; i++) {
            const auto e = ecs::entt()->create();
            ecs::entt()->emplace<dbg_out>(e, "text", i);
        }

        for (auto i = 0u; i < 10u; i++) {
            ZZ_INFO("update #{0}", i);
            update_ecs();
        }
    }
};

namespace zz::app {
    application *create_app() {
        return new ecs_app();
    }
}
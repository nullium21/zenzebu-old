#include "zz/app.h"
#include "zz/log.h"
#include "zz/ecs.h"

using namespace zz;
using namespace zz::app;

struct dbg_out {
    std::string text;
    uint32_t id;
};

class ecs_app: public application {
private:
    void update_ecs(entt::registry &registry) {
        auto view = registry.view<const dbg_out>();

        view.each([](const auto &out) {
            ZZ_INFO("{0} {1}", out.id, out.text);
        });
    }
public:
    void run() override {
        log::init();

        ZZ_INFO("starting ECS application");

        entt::registry registry;

        for (auto i = 0u; i < 10u; i++) {
            const auto e = registry.create();
            registry.emplace<dbg_out>(e, "test", i);
        }

        for (auto i = 0u; i < 10u; i++) {
            ZZ_INFO("update #{0}", i);
            update_ecs(registry);
        }
    }
};

namespace zz::app {
    application *create_app() {
        return new ecs_app();
    }
}
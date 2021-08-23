#include "zz/app.h"
#include "zz/log.h"
#include "zz/ecs.h"
#include "zz/layer.h"

using namespace zz;
using namespace zz::app;

class test_layer: public layer {
    string name() override { return "test layer"; }

    void on_update(layer_stack &st) override {
        ZZ_INFO("updating test layer");
    }
};

class layers_app: public application {
public:
    void run() override {
        log::init();
        ecs::init();

        auto ent = ecs::entt()->create();
        auto &ls = ecs::entt()->emplace<layer_stack>(ent);

        uint id = ls.push(new test_layer());

        for (auto i = 0u; i < 10u; i++)
            layering::update_all();
    }
};

namespace zz::app {
    application *create_app() {
        return new layers_app();
    }
}
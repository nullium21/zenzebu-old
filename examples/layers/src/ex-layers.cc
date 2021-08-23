#include "zz/app.h"
#include "zz/log.h"
#include "zz/ecs.h"
#include "zz/layer.h"

using namespace zz;
using namespace zz::app;

class test_layer: public layer {
    string name() override { return "test layer"; }

};

class layers_app: public application {
public:
    void run() override {
        log::init();
        ecs::init();

        auto ent = ecs::entt()->create();
        auto &ls = ecs::entt()->emplace<layer_stack>(ent);

        uint id = ls.push(new test_layer());
    }
};

namespace zz::app {
    application *create_app() {
        return new layers_app();
    }
}
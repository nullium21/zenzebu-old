#pragma once

#include "zz/ecs.h"

namespace zz::scripting {
    struct native_script {
        std::string name;

        std::function<void(entt::entity)> on_start;
        std::function<void(entt::entity)> on_update;
    };

    void update();
}

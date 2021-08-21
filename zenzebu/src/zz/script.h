#pragma once

#include "zz/ecs.h"

namespace zz::scripting {
    using script_callback = std::function<void(entt::entity)>;

    struct native_script {
        std::string name;

        script_callback on_start;
        script_callback on_update;
    };

    void update();
}

#pragma once

#include "zz/ecs.h"

namespace zz::scripting {
    struct native_script {
        std::string name;

        std::function<void()> on_start;
        std::function<void()> on_update;
    };

    void update();
}

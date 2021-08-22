#pragma once

#include "zz/ecs.h"

namespace zz::scripting {
    enum update_stage {
        us_oninit,
        us_onupdate
    };

    namespace native {
        using script_callback = std::function<void(entt::entity)>;

        struct on_init_script {
            script_callback on_init;
        };

        struct on_update_script {
            script_callback on_update;
        };
    }

    void update_scripts(update_stage);
}

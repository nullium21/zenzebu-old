#include "zz/log.h"
#include "zz/script.h"

using namespace zz;
using namespace zz::scripting;

void update(ecs::registry &registry) {
    auto native = registry.view<const native_script>();

    native.each([](const auto &script) {
        ZZ_CORE_INFO("updating native script '{0}'", script.name);

        script.on_update();
    });
}

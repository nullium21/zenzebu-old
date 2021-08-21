#include "zz/ecs.h"
#include "zz/log.h"
#include "zz/script.h"

using namespace zz::scripting;
using namespace zz::ecs;

void zz::scripting::update() {
    auto native = zz::ecs::ecs::entt()->view<const native_script>();

    native.each([](const auto entity, const auto &script) {
        ZZ_CORE_INFO("updating native script '{0}'", script.name);

        script.on_update(entity);
    });
}

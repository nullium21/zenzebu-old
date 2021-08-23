#include "zz/ecs.h"
#include "zz/log.h"
#include "zz/script.h"

using namespace zz::scripting;
using namespace zz;

#define NATIVE_UPDATE_STAGE(S) if (stage == update_stage::us_on##S) { \
    auto view = ecs::entt()->view<const native::on_##S##_script>(); \
    view.each([](const auto entity, const auto &script) { \
        script.on_##S(entity); \
    }); \
}

void update_native(update_stage stage) {
    NATIVE_UPDATE_STAGE(init);
    NATIVE_UPDATE_STAGE(update);
}

void zz::scripting::update_scripts(update_stage stage) {
    update_native(stage);
}

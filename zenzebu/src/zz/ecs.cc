#include "zz/ecs.h"
#include "zz/log.h"

using namespace zz::ecs;
using std::make_shared;

shared_ptr<registry> ecs::entt_reg;

void ecs::init() {
    ZZ_CORE_INFO("ECS initializing...");

    entt_reg = make_shared<registry>();
}

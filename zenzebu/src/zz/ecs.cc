#ifndef ZZ_USEPCH
#include "zz/log.h"
#endif

#include "zz/ecs.h"

using namespace zz;
using std::make_shared;

shared_ptr<registry> ecs::entt_reg;

void ecs::init() {
    ZZ_CORE_INFO("ECS initializing...");

    entt_reg = make_shared<registry>();
}

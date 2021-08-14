#include "zz/ecs.h"
#include "zz/log.h"

using namespace zz::ecs;

shared_ptr<registry> ecs::entt_reg;

void ecs::init() {
    ZZ_CORE_INFO("ECS initializing...");
}

entity ecs::new_entity() {
    return entt_reg->create();
}

template<typename T, typename... Args>
T& ecs::component(entity e, Args&&... args) {
    return entt_reg->emplace<T>(e, std::forward<Args>(args)...);
}

template<typename T>
T& ecs::get(entity e) {
    return entt_reg->get<T>(e);
}

template<typename... T>
void ecs::remove(entity e) {
    entt_reg->remove<T...>(e);
}

template<typename... T>
entt::view<T...> ecs::view() {
    return entt_reg->view<T...>();
}

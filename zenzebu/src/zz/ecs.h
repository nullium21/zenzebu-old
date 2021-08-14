#pragma once

#include "entt/entt.hpp"

#include <memory>

using std::shared_ptr;

namespace zz::ecs {

    using entt::registry;
    using entt::entity;
    using entt::view;

    class ecs {
    public:
        static void init();
        inline static shared_ptr<registry> &entt_registry() { return entt_reg; }

        static entity new_entity();

        template<typename T, typename... Args>
        static T& component(entity, Args&&...);

        template<typename T>
        static T& get(entity);

        template<typename... T>
        static void remove(entity);

        template<typename... T>
        static view<T...> view();
    private:
        static shared_ptr<registry> entt_reg;
    };
}

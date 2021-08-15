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
        inline static shared_ptr<registry> &entt() { return entt_reg; }
    private:
        static shared_ptr<registry> entt_reg;
    };
}

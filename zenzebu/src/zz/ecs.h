#pragma once

#ifndef ZZ_USEPCH
#include <memory>
#endif

#include "entt/entt.hpp"

// #include <memory>

using std::shared_ptr;

namespace zz {

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

    namespace component {
        struct parent {
            entity parent;
        };

        struct children {
            std::vector<entity> children;
        };
    }
}

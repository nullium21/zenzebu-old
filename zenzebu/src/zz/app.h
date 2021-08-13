#pragma once

namespace zz::app {
    class application {
    public:
        application();
        virtual ~application();

        virtual void run();
    };

    application *create_app();
}

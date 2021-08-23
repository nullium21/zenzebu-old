#pragma once

namespace zz::app {
    class application {
    public:
        application();
        virtual ~application();

        virtual void run();

    protected:
        bool running = true;
    };

    application *create_app();
}

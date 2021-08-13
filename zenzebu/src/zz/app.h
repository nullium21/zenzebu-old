#pragma once

namespace zz {
    class application {
    public:
        application();
        virtual ~application();

        virtual void run();
    };

    application *create_app();
}

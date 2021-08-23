#include "zz/app.h"
#include "zz/log.h"

using namespace zz;
using namespace zz::app;

class log_app: public application {
public:
    void run() override {
        log::init();

        ZZ_INFO("starting application");
    }
};

// log_app *zz::app::create_app() {
//     return new log_app();
// }

namespace zz::app {
    application *create_app() {
        return new log_app();
    }
}

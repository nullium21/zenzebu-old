#include "zz/app.h"
#include "zz/log.h"
#include "zz/ecs.h"

using namespace zz::app;

application::application() {}
application::~application() {}

void application::run() {
    while (running) {}
}

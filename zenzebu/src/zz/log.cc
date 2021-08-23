#include "zz/log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace zz {

    shared_ptr<spdlog::logger> log::clog;
    shared_ptr<spdlog::logger> log::mlog;

    void log::init() {
        spdlog::set_pattern("%^[%T %n] %v%$");

        clog = spdlog::stdout_color_mt("ZENZEBU");
        clog->set_level(spdlog::level::trace);

        mlog = spdlog::stdout_color_mt("APP");
        mlog->set_level(spdlog::level::trace);
    }

}

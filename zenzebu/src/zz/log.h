#pragma once

#include "spdlog/spdlog.h"

#include <memory>

using std::shared_ptr;

namespace zz {
    class log {
    public:
        static void init();
        inline static shared_ptr<spdlog::logger> &core_logger() { return clog; }
        inline static shared_ptr<spdlog::logger> &main_logger() { return mlog; }

    private:
        static shared_ptr<spdlog::logger> clog;
        static shared_ptr<spdlog::logger> mlog;
    };
}

#define ZZ_CORE_TRACE(...)  ::zz::log::core_logger()->trace(__VA_ARGS__)
#define ZZ_CORE_INFO(...)   ::zz::log::core_logger()->info(__VA_ARGS__)
#define ZZ_CORE_WARN(...)   ::zz::log::core_logger()->warn(__VA_ARGS__)
#define ZZ_CORE_ERROR(...)  ::zz::log::core_logger()->error(__VA_ARGS__)
#define ZZ_CORE_FATAL(...)  ::zz::log::core_logger()->fatal(__VA_ARGS__)

#define ZZ_TRACE(...)       ::zz::log::main_logger()->trace(__VA_ARGS__)
#define ZZ_INFO(...)        ::zz::log::main_logger()->info(__VA_ARGS__)
#define ZZ_WARN(...)        ::zz::log::main_logger()->warn(__VA_ARGS__)
#define ZZ_ERROR(...)       ::zz::log::main_logger()->error(__VA_ARGS__)
#define ZZ_FATAL(...)       ::zz::log::main_logger()->fatal(__VA_ARGS__)

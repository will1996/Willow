#include "willow/root/Logger.hpp"
#include<memory>
#include<spdlog/sinks/basic_file_sink.h>
#include<spdlog/sinks/stdout_color_sinks.h>

namespace wlo{
        std::shared_ptr<spdlog::logger> logr::s_CoreLogger;
        std::shared_ptr<spdlog::logger> logr::s_AppLogger;
        bool wlo::logr::initialized = false;
    void logr::initalize(){
        if(!initialized) {
            auto console_core_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            console_core_sink->set_level(spdlog::level::trace);
            console_core_sink->set_pattern("Willo_Core: [%^%l%$] %v");
            s_CoreLogger = std::make_shared<spdlog::logger>("Willo_Core", console_core_sink);

            auto console_app_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            console_app_sink->set_level(spdlog::level::trace);
            console_app_sink->set_pattern("Willo_App: [%^%l%$] %v");
            s_AppLogger = std::make_shared<spdlog::logger>("Willo_Application", console_app_sink);
            initialized = true;
        }
    }
}






#pragma once
#include "spdlog/spdlog.h"
#include<memory>
//Hazel style logger
namespace wlo{
        class logr{
            public:
            static void initalize();
            static void reclaim();
            static std::shared_ptr<spdlog::logger>& getCoreLogger(){
                if(!initialized)
                    throw std::runtime_error("Call to core logger before initialize, call wlo::logr::initialize");
                return s_CoreLogger;
            }
            static std::shared_ptr<spdlog::logger>& getAppLogger(){
                if(! initialized)
                throw std::runtime_error("Call to app logger before initialize, call wlo::logr::initialize");
                return s_AppLogger;
            }
            private:
            static bool initialized;
            logr(){

            };
            ~logr(){};
            static std::shared_ptr<spdlog::logger> s_CoreLogger;
            static std::shared_ptr<spdlog::logger> s_AppLogger;
            

        };
    
}
//core logging functions
#define WILO_CORE_INFO(...) ::wlo::logr::getCoreLogger()->info(__VA_ARGS__);
#define WILO_CORE_WARNING(...) ::wlo::logr::getCoreLogger()->warn(__VA_ARGS__)
#define WILO_CORE_ERROR(...) ::wlo::logr::getCoreLogger()->error(__VA_ARGS__);
#define WILO_CORE_CRITICAL(...) ::wilo::logr::getCoreLogger()->critical(__VA_ARGS__);
//app_level logging functions
#define WILO_INFO(...) ::wlo::logr::getAppLogger()->info(__VA_ARGS__);
#define WILO_WARNING(...) ::wilo::logr::getAppLogger()->warn(__VA_ARGS__)
#define WILO_ERROR(...) ::wilo::logr::getAppLogger()->error(__VA_ARGS__);
#define WILO_CRITICAL(...) ::wilo::logr::getAppLogger()->critical(__VA_ARGS__);
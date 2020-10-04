#pragma once
#include "willow/root/wilo_dev_core.hpp"
#include "willow/messaging/wilo_observer_system.hpp"
#include "willow/window/wilo_window.hpp"
#include "willow/scripting/Scriptable.hpp"
#include "wilo_console.hpp"
#include "willow/rendering/wilo_renderer.hpp"
#include "willow/scripting/LuaBasics.h"
namespace wlo{
    /*

    
    
    */
    class Application: public Observer{
        public:

            struct Info{
            std::string appName;    
            bool debugging;
            int appVersion;
            const double engineVersion = WILO_ENGINE_VERSION;
            const std::string engineName = "";
            Info(std::string name, int version) :appName(name),appVersion(version){}
            };

            Application();
            virtual void recieve(const wlo::Message& msg);
            virtual void recieve(const wlo::WindowMessage& msg);
            virtual void initialize();
            void run();
            virtual void reclaim();
            virtual ~Application();
        private:
            wlo::SharedPointer<Window> m_main_window;
            wlo::SharedPointer<wlo::lua::Environment> m_scriptEnv;
            wlo::UniquePointer<Console> m_main_console;
            wlo::UniquePointer<Renderer> m_renderer;
            
            size_t m_id = 0;
            bool m_shutting_down = false;
            float xTranslation = 0.0;
            float yTranslation = 0.0;
        
    };
    
}

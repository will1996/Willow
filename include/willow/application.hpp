#pragma once
#include "willow/root/wilo_dev_core.hpp"
#include "willow/messaging/MessageSystem.hpp"
#include "willow/window/wilo_window.hpp"
#include "willow/scripting/LuaBinding.hpp"
#include "willow/console/wilo_console.hpp"
#include "willow/rendering/wilo_renderer.hpp"
#include "willow/scripting/LuaEnvironment.h"
namespace wlo{
    /*
    Application is the entry-point to the game engine. Anything extending Application has access to
     the full breadth of Willow.

     Application is intended to be created through the WILLOW_TRUNK macro, doing so ensures that the appropriate
     runtime informatino is passed to it, If however, you have different plans, see willow.hpp to see how WILLOW_TRUNK
     works. It's quite simple, and if you need to bypass it, you should easily be able to.
    
    
    */
class Application: public MessageSystem::Observer{
        public:

            struct Info{
            std::string appName;    
            bool debugging;
            int appVersion;
            const double engineVersion = WILO_ENGINE_VERSION;
            const std::string engineName = "";
            Info(std::string name, int version) :appName(name),appVersion(version){}
            };

            Application(std::string argv_0);//Application must recieve argv[0] in its constructor, it uses this path to
            //handle runtime file lookup, without this nothing works.

            virtual bool isInitialized();
            virtual void recieve(const wlo::KeyboardMessage& msg);
            virtual void recieve(const wlo::WindowMessage& msg);
            virtual void initialize(bool reinitialize = false);
            virtual void initialize(Application::Info inf,bool reinitialize = false);
            virtual void run();
            virtual void reclaim();
            virtual ~Application();

        protected:
            wlo::SharedPointer<Window> m_main_window;
            wlo::SharedPointer<wlo::lua::Environment> m_scriptEnv;
            wlo::UniquePointer<Console> m_console;
            wlo::UniquePointer<Renderer> m_renderer;


        std::string m_runtimeRoot;
            size_t m_id = 0;
            bool m_shutting_down = false;
            bool m_windowResized = false;
            bool m_initialized;
    };
    
}

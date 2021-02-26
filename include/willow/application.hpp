#pragma once
#include "willow/root/FileSystem.hpp"
#include "willow/root/Root.hpp"
#include "willow/messaging/MessageSystem.hpp"
#include "willow/window/Window.hpp"
#include "willow/scripting/LuaBinding.hpp"
#include "willow/console/wilo_console.hpp"
#include "willow/rendering/Renderer.hpp"
#include "willow/scripting/LuaEnvironment.h"
namespace wlo{
    /*
    Application is the entry-point to the game engine. Anything extending Application has access to
     the full breadth of Willow.

     Application is intended to be created through the WILLOW_TRUNK macro, doing so ensures that the appropriate runtime informatino is passed to it, If however, you have different plans, see willow.hpp to see how WILLOW_TRUNK works. It's quite simple, and if you need to bypass it, you should easily be able to.
    
    */
class Application: public MessageSystem::Observer{
        public:

            struct Info{
            std::string appName;    
            bool debugging;
            int appVersion;
            Info(std::string name, int version) :appName(name),appVersion(version){}
            };

            Application(Info,std::string argv_0);//Application must recieve argv[0] in its constructor, it uses this path to
            //handle runtime file lookup, without this nothing works.

            virtual void recieve(const wlo::KeyboardMessage& msg);
            void recieve(const wlo::WindowClosed& msg);
            virtual void recieve(const rendering::GPUInfo & msg);
            virtual void setup() = 0;
            virtual void stepSim(float elapsedTime) = 0;
            virtual void draw() =0;
            void shutdown();
            void run();
            virtual void reclaim();
            virtual ~Application();

        protected:
            decltype(std::chrono::high_resolution_clock::now()) startTime;
            float timeElapsed();
            virtual void initialize(Application::Info inf);
            wlo::SharedPointer<Window> m_main_window;
            wlo::lua::Environment m_scriptEnv;
            wlo::UniquePointer<Console> m_console;
            wlo::UniquePointer<rendering::Renderer> m_renderer;


        std::string m_runtimeRoot;
            size_t m_id = 0;
            bool m_shutting_down = false;
            bool m_windowResized = false;
    };
    
}

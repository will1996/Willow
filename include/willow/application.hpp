#pragma once
#include "willow/root/FileSystem.hpp"
#include "willow/root/Root.hpp"
#include "willow/messaging/MessageSystem.hpp"
#include "willow/window/Window.hpp"
#include "willow/scripting/LuaBinding.hpp"
#include "willow/console/wilo_console.hpp"
#include "willow/rendering/Renderer.hpp"
#include "willow/scripting/LuaEnvironment.hpp"
#include"willow/input/InputManager.hpp"
#include"willow/rendering/PerspectiveCamera3D.hpp"
#include "willow/Assets.hpp"
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
            int appVersion = 0;
            bool debugging = true;
            wlo::Vec2 windowDimensions = {500,500};
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
            Window m_mainWindow;

            ScriptEnvironment m_scriptEnv;
            Assets m_assets;

            InputManager m_input;
            rendering::Renderer m_renderer;
            PerspectiveCamera3D m_mainCamera;
//            wlo::UniquePointer<Console> m_console;


        std::string m_runtimeRoot;
            size_t m_id = 0;
            bool m_shutting_down = false;
            bool m_windowResized = false;
    };
    
}

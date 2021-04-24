#include "willow/Application.hpp"
#include<iostream>
#include "willow/root/Root.hpp"
#include "willow/window/Window.hpp"
#include "willow/scripting/LuaBinding.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace wlo{
 Application::Application(Application::Info info,std::string argv_0)
 :
 m_mainWindow({.m_height = 500,.m_width = 500, .m_title = info.appName,.API = wlo::WindowingAPICode::GLFW,}),
m_assets(m_scriptEnv),
 m_mainCamera(m_mainWindow),
 m_renderer(m_scriptEnv,m_mainWindow,{}),
 m_input(m_mainWindow),
 startTime(std::chrono::high_resolution_clock::now())
 {
     fs::path argv0 = fs::path(argv_0);
     //build the logger internals
     wlo::logr::initalize();
     //set the root of the filesystem to 
     FileSystem::initialize(argv0.parent_path());
//     m_console = wlo::CreateUniquePointer<wlo::Console>(m_scriptEnv);
     m_mainWindow.permit<KeyboardMessage, Application,&Application::recieve>(this) ;//register as an observer with the window, so we recieve events;

     m_renderer.asSubject().permit<rendering::GPUInfo,Application,&Application::recieve>(this);
     m_renderer.setMainCamera(m_mainCamera);

     WILO_CORE_INFO("application initialized!");

//     m_scriptEnv = wlo::CreateSharedPointer<wlo::lua::Environment>();
 }
 Application::~Application(){
     reclaim();
 }

    void Application::recieve(const wlo::KeyboardMessage& msg){
        if(msg.content.button==wlo::Key::Code::ESCAPE) {
            WILO_CORE_INFO("Application shutting down");
            m_shutting_down = true;
        }

    }

    void Application::run(){
     WILO_CORE_INFO("application running!");
     setup();
     double dt =0;
     while(!m_mainWindow.shouldClose()&&!m_shutting_down){
        auto startTime =std::chrono::high_resolution_clock::now();
        stepSim(dt);
        draw();
        auto curTime= std::chrono::high_resolution_clock::now();
        dt = std::chrono::duration<float,std::chrono::seconds::period>(startTime-curTime).count();
     }
 }



 void Application::reclaim(){
 //    m_main_console->reclaim();//destroy console
     Observer::reclaim();//free observer resources
     //m_renderer->reclaim();
     
     WILO_CORE_INFO("reclamation complete")
 }


    void Application::recieve(const WindowClosed &msg) {
               if(msg.content.action==WindowAction::Closed) {
                   WILO_CORE_INFO("Application shutting down");
                   m_shutting_down = true;
               }
    }

    void Application::recieve(const rendering::GPUInfo & msg){
            cout<<"GPU CURRENTLY USING "<<msg.totalMemoryUsage<<" BYTES OF "<<msg.totalMemoryAvailable<<endl;
    }

    float Application::timeElapsed() {
        auto currentTime = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
    }
    void Application::shutdown() {

    }


}

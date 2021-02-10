#include "willow/application.hpp"
#include<iostream>
#include "willow/root/wilo_dev_core.hpp"
#include "willow/window/wilo_window.hpp"
#include "willow/scripting/LuaBinding.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace wlo{
 Application::Application(Application::Info info)
 :
 startTime(std::chrono::high_resolution_clock::now())
 {
     wlo::logr::initalize();
     initialize(info);
//     m_scriptEnv = wlo::CreateSharedPointer<wlo::lua::Environment>();
 }
 Application::~Application(){
     reclaim();
 }
 void Application::initialize(Application::Info inf){
     m_console = wlo::CreateUniquePointer<wlo::Console>(m_scriptEnv);

     wlo::Window::Info window_info;
     window_info.m_height = 500;
     window_info.m_width = 500;
     window_info.m_title = inf.appName;
     window_info.API = wlo::WindowingAPICode::GLFW;
     m_main_window = wlo::SharedPointer<Window>(wilo_get_window(window_info));
     m_main_window->permit<KeyboardMessage, Application,&Application::recieve>(this) ;//register as an observer with the window, so we recieve events;

     m_renderer = wlo::CreateUniquePointer<wlo::rendering::Renderer>(m_main_window);
   WILO_CORE_INFO("application initialized!");
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
     while(!m_main_window->shouldClose()&&!m_shutting_down){
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

    float Application::timeElapsed() {
        auto currentTime = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
    }

    void Application::shutdown() {

    }


}

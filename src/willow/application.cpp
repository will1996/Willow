#include "willow/application.hpp"
#include<iostream>
#include "willow/root/wilo_dev_core.hpp"
#include "willow/window/wilo_window.hpp"
#include "willow/scripting/Scriptable.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace wlo{
 Application::Application()
 {
     wlo::logr::initalize();
//     m_scriptEnv = wlo::CreateSharedPointer<wlo::lua::Environment>();
//     m_main_console = wlo::CreateUniquePointer<wlo::Console>(m_scriptEnv);
 }
 Application::~Application(){
     //do nothing, as is our way. 
 }
 void Application::initialize(Application::Info inf){

     wlo::Window::Info window_info;
     window_info.m_height = 600;
     window_info.m_width = 800;
     window_info.m_title = inf.appName;

     m_main_window = wlo::SharedPointer<Window>(wilo_get_window(window_info));
     m_main_window->initialize();//initialize a platform specific window, making indirected calls
     m_main_window->permit<KeyboardMessage, Application,&Application::recieve>(this) ;//register as an observer with the window, so we recieve events;
     //m_main_window->permit<MouseMessage,Application,&Application::recieve>(this) ;//register as an observer with the window, so we recieve events;
     m_main_window->permit<WindowMessage,Application,&Application::recieve>(this) ;//register as an observer with the window, so we recieve events;

//     m_main_console->initialize();
//     m_main_console->asSubject()->permit<WindowMessage,Application,&Application::recieve>(this);
//     m_main_console->asSubject()->permit<MouseMessage,Application,&Application::recieve>(this);
//     m_main_console->asSubject()->permit<KeyboardMessage,Application,&Application::recieve>(this);
     wlo::Renderer::Info rendererInfo;
     rendererInfo.frameHeight = 600;
     rendererInfo.frameHeight = 800;
     rendererInfo.enableGraphicsDebugging = true;
    m_renderer = wlo::CreateUniquePointer<Renderer>(m_main_window, rendererInfo);
    m_main_window->permit<WindowMessage,Renderer,&Renderer::handleWindowResize>(m_renderer.get());
   WILO_CORE_INFO("application initialized!");
 }
 void Application::run(){
     WILO_CORE_INFO("application running!");
     while(!m_shutting_down){
        // m_main_console->checkIn();
//        m_main_console->checkIn();
        m_main_window->checkIn();//get all these events goin'
        m_renderer->beginDrawCall();
        m_renderer->submitDrawCall();
     }
 }

    void Application::recieve(const wlo::WindowMessage& msg){
        if(msg.getType()==MessageType::WindowClose){
            WILO_CORE_INFO("Application shutting down");
            m_shutting_down = true;
        }

 }
    void Application::recieve(const wlo::KeyboardMessage& msg){
      if(msg.getType()==MessageType::KeyPressed){
          wlo::KeyboardMessage::Info keyInfo = msg.getInfo();
      }
     }
 


 void Application::reclaim(){
 //    m_main_console->reclaim();//destroy console
     Observer::reclaim();//free observer resources
     m_main_window->reclaim();//destroy window
     m_renderer->reclaim();
     
     WILO_CORE_INFO("reclamation complete")
 }



}

#include "willow/application.hpp"
#include<iostream>
#include "willow/root/wilo_dev_core.hpp"
#include "willow/window/wilo_window.hpp"
#include "willow/scripting/LuaBinding.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace wlo{
 Application::Application(std::string argv_0) : m_runtimeRoot(argv_0)
 {
     wlo::logr::initalize();
//     m_scriptEnv = wlo::CreateSharedPointer<wlo::lua::Environment>();
 }
 Application::~Application(){
     reclaim();
 }
 void Application::initialize(Application::Info inf, bool reinitialize){
     if(m_initialized && !reinitialize){
        WILO_CORE_WARNING("Called Application::initialize but application is already intiailzied, to force this, set reinitialize optional argument to true");
         return;
     }
        //initialization order is unimportant outside of draw order, the main window is intialized last, this way
        //it is drawn on top


     m_scriptEnv = wlo::CreateSharedPointer<wlo::lua::Environment>();
     m_console = wlo::CreateUniquePointer<wlo::Console>(m_scriptEnv);
     m_console->initialize();
     m_console->asSubject()->permit<WindowMessage,Application,&Application::recieve>(this);
     m_console->asSubject()->permit<KeyboardMessage,Application,&Application::recieve>(this);

     wlo::Window::Info window_info;
     window_info.m_height = 600;
     window_info.m_width = 800;
     window_info.m_title = inf.appName;
     m_main_window = wlo::SharedPointer<Window>(wilo_get_window(window_info));
     m_main_window->permit<KeyboardMessage, Application,&Application::recieve>(this) ;//register as an observer with the window, so we recieve events;
     //m_main_window->permit<MouseMessage,Application,&Application::recieve>(this) ;//register as an observer with the window, so we recieve events;
     m_main_window->permit<WindowMessage,Application,&Application::recieve>(this) ;//register as an observer with the window, so we recieve events;

   // m_renderer = wlo::CreateUniquePointer<Renderer>(m_main_window, rendererInfo);
    //m_main_window->permit<WindowResized,Renderer,&Renderer::handleWindowResize>(m_renderer.get());

    m_initialized = true;
   WILO_CORE_INFO("application initialized!");
 }
 void Application::run(){
     WILO_CORE_INFO("application running!");
     while(!m_shutting_down){
        // m_main_console->checkIn();
//        m_main_console->checkIn();
        m_main_window->checkIn();//get all these events goin'
     }
 }

    void Application::recieve(const wlo::WindowMessage& msg){
        if(msg.content.action==WindowAction::Closed) {
            WILO_CORE_INFO("Application shutting down");
            m_shutting_down = true;
        }
        if(msg.content.action==WindowAction::Resized)
            m_windowResized = true;
 }
    void Application::recieve(const wlo::KeyboardMessage& msg){

     }



 void Application::reclaim(){
 //    m_main_console->reclaim();//destroy console
     Observer::reclaim();//free observer resources
     //m_renderer->reclaim();
     
     WILO_CORE_INFO("reclamation complete")
 }

    void Application::initialize(bool reinitialize) {
        Info defaultAppInfo("Untitled Willow App",0);
#ifndef  NDEBUG
        defaultAppInfo.debugging = true;
#endif
        initialize(defaultAppInfo,reinitialize);
    }

    bool Application::isInitialized() {
     return   m_initialized;
    }


}


#include "willow/Application.hpp"
#include<iostream>
#include "willow/root/Root.hpp"
#include "willow/window/Window.hpp"
#include"willow/rendering/Renderer.hpp"
#include"willow/input/InputManager.hpp"
#include"willow/ecs/EntityComponentSystem.hpp"
#include"willow/data/Value.hpp"
#include "willow/scripting/LuaBinding.hpp"
#include"willow/root/Logger.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace wlo{
 Application::Application(std::string argv_0)
 :
 m_core({}),
    startTime(std::chrono::high_resolution_clock::now())
 {

     auto pwind = CreateUniquePointer<Window>(Window::Info{});
     pwind->permit<WindowClosed,Application,&Application::stop>(this);

     auto pinput = CreateUniquePointer<InputManager>();
     auto plogger = CreateUniquePointer<Logger>();
     plogger->connect(this);
     m_core.addComponent(std::move(pwind));
     m_core.addComponent(std::move(plogger));

 }
    Application::~Application(){
     Observer::reclaim();//free observer resources
 }



 void Application::run(){
     m_core.mainLoop();
 }

    float Application::timeElapsed() {
        auto currentTime = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
    }

    void Application::stop(const WindowClosed &) {
        m_core.stop();
    }



}

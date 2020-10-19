#pragma once
#include "willow/root/wilo_dev_core.hpp"
#include "willow/messaging/Messages.hpp"
#include "window.hpp"

namespace wlo{

    class MacWindowImpl;

    class MacWindow : public Window{
        public:
         
         MacWindow(Window::Info &info);
         wlo::Window::Info getInfo() const override;
         void initialize()  ;
         void checkIn()     override;
         void reclaim()     ;
         bool shouldClose() override;
         void* getNativeWindow() override;
        
        private:
            wlo::UniquePointer<MacWindowImpl> p_impl;
            void notifyKeyObservers(const wlo::KeyboardMessage& msg);
            void notifyMouseObservers(const wlo::MouseMoved& msg);
            void notifyMouseObservers(const wlo::MouseScrolled& msg);
            void notifyMouseObservers(const wlo::MouseButtonMessage& msg);
            void notifyWindowObservers(const wlo::WindowMessage& msg);
            Window::Info m_info;
            
            

    };
}
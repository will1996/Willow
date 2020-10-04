#pragma once
#include "willow/root/wilo_dev_core.hpp"
#include "willow/messaging/keyboard_message.hpp"
#include "willow/messaging/mouse_message.hpp"
#include "willow/messaging/window_message.hpp"
#include "window.hpp"

namespace wlo{

    class MacWindowImpl;

    class MacWindow : public Window{
        public:
         
         MacWindow(Window::Info &info);
         wlo::Window::Info getInfo() const override;
         void initialize()  override;
         void checkIn()     override;
         void reclaim()     override;
         bool shouldClose() override;
         void* getNativeWindow() override;
        
        private:
            wlo::UniquePointer<MacWindowImpl> p_impl;
            void notifyKeyObservers(const wlo::KeyboardMessage& msg);
            void notifyMouseObservers(const wlo::MouseMessage& msg);
            void notifyWindowObservers(const wlo::WindowMessage& msg);
            Window::Info m_info;
            
            

    };
}
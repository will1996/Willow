#pragma once
#include "willow/root/wilo_dev_core.hpp"
#include "willow/messaging/Messages.hpp"
#include "window.hpp"

namespace wlo{

    class MacWindowImpl;

    class MacWindow : public Window{

        public:

        MacWindow(struct Info info);

        ~MacWindow();
            wlo::Window::Info getInfo() const override;
            void initialize()  ;
            void checkIn()     override;
            void reclaim();
            bool shouldClose(bool queryAutomatically = true) override;
            void* getNativeWindow() const override;
        
        private:
            wlo::UniquePointer<MacWindowImpl> p_impl;
            void notifyKeyObservers(const wlo::KeyboardMessage& msg);
            void notifyMouseObservers(const wlo::MouseMoved& msg);
            void notifyMouseObservers(const wlo::MouseScrolled& msg);
            void notifyMouseObservers(const wlo::MouseButtonMessage& msg);
            void notifyWindowObservers(const wlo::WindowResized& msg);
            void notifyWindowObservers(const wlo::WindowClosed& msg);
            void notifyWindowObservers(const wlo::WindowLostFocus& msg);
            void notifyWindowObservers(const wlo::WindowGainedFocus& msg);
        Window::Info m_info;
    };

}
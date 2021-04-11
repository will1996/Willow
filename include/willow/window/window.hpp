#pragma once
#include "willow/messaging/MessageSystem.hpp"
namespace wlo{
class WindowImpl;

class Window: public MessageSystem::Subject{
        public:
        struct Info{
            uint32_t m_height = 600;
            uint32_t m_width = 800;
            std::string m_title = "Willo App";
            wlo::WindowingAPICode API;
            void * m_extraData = nullptr;//here for platform specific info that may need to be added
        };
          explicit Window(Info info);
          ~Window();
          Info getInfo()const;
          void checkIn();
          bool shouldClose(bool queryMeessagesAutomatically = true);
          void* getNativeWindow() const;
          void setCursorMode(bool locked);
          protected:
          Window();
private:
          void initialize();
          wlo::UniquePointer<WindowImpl> p_impl;
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

#pragma once
#include "willow/messaging/MessageSystem.hpp"
namespace wlo{
class Window: public MessageSystem::Subject{
        public:
        struct Info{
            uint32_t m_height = 600;
            uint32_t m_width = 800;
            std::string m_title = "Willo App";
            wlo::WindowingAPICode API;
            void * m_extraData = nullptr;//here for platform specific info that may need to be added
        };
          Window(){wlo::logr::initalize();};
          virtual ~Window(){};
          virtual Info getInfo() const = 0;
          virtual void checkIn() = 0;
          virtual bool shouldClose(bool queryMeessagesAutomatically = true) = 0;
          virtual void* getNativeWindow() const = 0;
          virtual void setCursorMode(bool locked) = 0;
    };

}

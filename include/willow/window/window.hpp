#pragma once
#include "willow/messaging/MessageSystem.hpp"
namespace wlo{
class Window: public MessageSystem::Subject{
        public:
        struct Info{
            double m_height = 600;
            double m_width = 800;
            std::string m_title = "Willo App";
            wlo::WindowingAPICode API;
            void * m_extraData = nullptr;//here for platform specific info that may need to be added
        };
          virtual ~Window(){};
          virtual Info getInfo() const = 0; 
          virtual void checkIn() = 0;
          virtual bool shouldClose() = 0;
          virtual void* getNativeWindow() = 0;
    };

}
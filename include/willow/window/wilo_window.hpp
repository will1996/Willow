#pragma once
#include "mac_window.hpp"
namespace wlo{
    //TODO make other platforms available
    inline Window* wilo_get_window(Window::Info& info){
        info.API = wlo::WindowingAPICode::GLFW;
            return new MacWindow(info);
    }
}
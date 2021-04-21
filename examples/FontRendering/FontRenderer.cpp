//
// Created by W on 10/9/20.
//
#include"willow/rendering/Renderer.hpp"
#include"willow/console/Font.hpp"
using namespace wlo;
int main(){
Window wnd({.m_height=600,.m_width=700,.m_title = "FontRendering",.API = wlo::WindowingAPICode::GLFW});
rendering::Renderer renderer(wnd);
Font f("hello");
while(!wnd.shouldClose()){
}

}
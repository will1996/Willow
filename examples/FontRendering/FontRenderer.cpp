//
// Created by W on 10/9/20.
//
#include"willow/rendering/Renderer.hpp"
#include"willow/console/Font.hpp"
using namespace wlo;
int main(){
Window wnd({.m_title = "FontRendering",.m_width=600,.m_height=700,.API = wlo::WindowingAPICode::GLFW});
rendering::Renderer renderer(wnd);
Font f("hello");
while(!wnd.shouldClose()){
}

}
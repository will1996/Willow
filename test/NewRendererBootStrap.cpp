//
// Created by W on 11/3/20.
//
#include "willow/rendering/Renderer.hpp"
#include "willow/scripting/LuaEnvironment.hpp"
#include "willow/window/Window.hpp"
#include "willow/rendering/Buffer.hpp"
#include <filesystem>
using namespace  wlo::rendering;
using namespace wlo;
struct ColorVertex3D{
    Vec3 position;
    Vec4 color;
};
int main(){
    //first, let's make something to render to:
    Window::Info windowInfo{
            .m_height = 500,
            .m_width = 500,
            .m_title = "rendering setup test",
            .API = WindowingAPICode::GLFW
    };
Window window(windowInfo);//the window is going to be partially owned by the renderer, so make a shared pointer


//now, lets create the most basic renderer, it requires a window
//you can specify optional features here, things like triangle fans, poly lines, ray tracing, ect.  (currently there are none supported
wlo::rendering::Renderer renderer();
 std::vector<ColorVertex3D> VertexBuffer(
            {
                    // red face
                    {.position = {-1.0f, -1.0f, 1.0f}, .color = {1.0f, 0.0f, 0.0f, 1.0f}},
                    {.position = {-1.0f, 1.0f,  1.0f}, .color = {1.0f, 0.0f, 0.0f, 1.0f}},
                    {.position = {1.0f,  -1.0f, 1.0f}, .color = {1.0f, 0.0f, 0.0f, 1.0f}},
                    {.position = {1.0f,  -1.0f, 1.0f}, .color = {1.0f, 0.0f, 0.0f, 1.0f}},
                    {.position = {-1.0f, 1.0f,  1.0f}, .color = {1.0f, 0.0f, 0.0f, 1.0f}},
                    {.position = {1.0f,  1.0f,  1.0f}, .color = {1.0f, 0.0f, 0.0f, 1.0f}},
                    // green face
                    {.position = {-1.0f, -1.0f, -1.0f}, .color = {0.0f, 1.0f, 0.0f, 1.0f}},
                    {.position = {1.0f,  -1.0f, -1.0f}, .color = {0.0f, 1.0f, 0.0f, 1.0f}},
                    {.position = {-1.0f, 1.0f,  -1.0f}, .color = {0.0f, 1.0f, 0.0f, 1.0f}},
                    {.position = {-1.0f, 1.0f,  -1.0f}, .color = {0.0f, 1.0f, 0.0f, 1.0f}},
                    {.position = {1.0f,  -1.0f, -1.0f}, .color = {0.0f, 1.0f, 0.0f, 1.0f}},
                    {.position = {1.0f,  1.0f,  -1.0f}, .color = {0.0f, 1.0f, 0.0f, 1.0f}},
                    //{ blue face
                    {.position = {-1.0f, 1.0f,  1.0f}, .color = {0.0f, 0.0f, 1.0f, 1.0f}},
                    {.position = {-1.0f, -1.0f, 1.0f}, .color = {0.0f, 0.0f, 1.0f, 1.0f}},
                    {.position = {-1.0f, 1.0f,  -1.0f}, .color = {0.0f, 0.0f, 1.0f, 1.0f}},
                    {.position = {-1.0f, 1.0f,  -1.0f}, .color = {0.0f, 0.0f, 1.0f, 1.0f}},
                    {.position = {-1.0f, -1.0f, 1.0f}, .color = {0.0f, 0.0f, 1.0f, 1.0f}},
                    {.position = {-1.0f, -1.0f, -1.0f}, .color = {0.0f, 0.0f, 1.0f, 1.0f}},
                    // yellow face
                    {.position = {1.0f,  1.0f,  1.0f}, .color = {1.0f, 1.0f, 0.0f, 1.0f}},
                    {.position = {1.0f,  1.0f,  -1.0f}, .color = {1.0f, 1.0f, 0.0f, 1.0f}},
                    {.position = {1.0f,  -1.0f, 1.0f}, .color = {1.0f, 1.0f, 0.0f, 1.0f}},
                    {.position = {1.0f,  -1.0f, 1.0f}, .color = {1.0f, 1.0f, 0.0f, 1.0f}},
                    {.position = {1.0f,  1.0f,  -1.0f}, .color = {1.0f, 1.0f, 0.0f, 1.0f}},
                    {.position = {1.0f,  -1.0f, -1.0f}, .color = {1.0f, 1.0f, 0.0f, 1.0f}},
                    //{ magenta face
                    {.position = {1.0f,  1.0f,  1.0f}, .color = {1.0f, 0.0f, 1.0f, 1.0f}},
                    {.position = {-1.0f, 1.0f,  1.0f}, .color = {1.0f, 0.0f, 1.0f, 1.0f}},
                    {.position = {1.0f,  1.0f,  -1.0f}, .color = {1.0f, 0.0f, 1.0f, 1.0f}},
                    {.position = {1.0f,  1.0f,  -1.0f}, .color = {1.0f, 0.0f, 1.0f, 1.0f}},
                    {.position = {-1.0f, 1.0f,  1.0f}, .color = {1.0f, 0.0f, 1.0f, 1.0f}},
                    {.position = {-1.0f, 1.0f,  -1.0f}, .color = {1.0f, 0.0f, 1.0f, 1.0f}},
                    //{ cyan face
                    {.position = {1.0f,  -1.0f, 1.0f}, .color = {0.0f, 1.0f, 1.0f, 1.0f}},
                    {.position = {1.0f,  -1.0f, -1.0f}, .color = {0.0f, 1.0f, 1.0f, 1.0f}},
                    {.position = {-1.0f, -1.0f, 1.0f}, .color = {0.0f, 1.0f, 1.0f, 1.0f}},
                    {.position = {-1.0f, -1.0f, 1.0f}, .color = {0.0f, 1.0f, 1.0f, 1.0f}},
                    {.position = {1.0f,  -1.0f, -1.0f}, .color = {0.0f, 1.0f, 1.0f, 1.0f}},
                    {.position = {-1.0f, -1.0f, -1.0f}, .color = {0.0f, 1.0f, 1.0f, 1.0f}},
            });


glm::mat4x4 modelMatrx1{1};
glm::mat4x4 modelMatrx2{1};
glm::mat4x4 modelMatrx3{1};
glm::mat4x4 modelMatrx4{1};

//You need to have specified all of the resources for the frame class ahead of time (similar to a scene)
//basically, create a frame that represents the worst case scenario, and submit that. If you fail to do that, segfaults are bound to happen. 

// a frame is a sequence of events, all of which culminate in creating a frame on the screen sometime after submission
auto start = std::chrono::high_resolution_clock::now();
while(!window.shouldClose()) {
    //once allocated you can submit any frame with the same render paths, and up to the number of vertices in the example frame. 
auto delta = std::chrono::high_resolution_clock::now() -start;
    window.checkIn();
    modelMatrx1 = glm::rotate(glm::mat4(1.0f),  delta.count()*glm::radians(90.0f), glm::normalize(glm::vec3(0.0f, 1.0f, 1.0f)));
    modelMatrx2 = glm::translate(glm::mat4x4(1), sinf(delta.count())*glm::vec3(0, 3, 0));//*glm::rotate(glm::mat4(1.0f),  0.01f*delta.count()*glm::radians(90.0f), glm::normalize(glm::vec3(0.0f, 1.0f, 1.0f)));
    modelMatrx3 = glm::translate(glm::mat4x4(1), sinf(delta.count())*glm::vec3(3, 0, 0));//*glm::rotate(glm::mat4(1.0f),  0.01f*delta.count()*glm::radians(90.0f), glm::normalize(glm::vec3(0.0f, 1.0f, 1.0f)));
    modelMatrx4 = glm::translate(glm::mat4x4(1), sinf(delta.count())*glm::vec3(0, 0, 3));//*glm::rotate(glm::mat4(1.0f),  0.01f*delta.count()*glm::radians(90.0f), glm::normalize(glm::vec3(0.0f, 1.0f, 1.0f)));
}
//all data referenced by draws in the frame is copied to a the gpu by the time submit returns, so attachments are
//safe to write to immediately after submission.
};











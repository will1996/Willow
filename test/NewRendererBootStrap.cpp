//
// Created by W on 11/3/20.
//
#include "willow/rendering/Renderer.hpp"
#include "willow/window/mac_window.hpp"
#include "willow/rendering/Buffer.hpp"
#include <filesystem>
using namespace  wlo::rendering;
using namespace wlo;
int main(){
    //first, let's make something to render to:
    MacWindow::Info windowInfo{
            .m_height = 500,
            .m_width = 500,
            .m_title = "rendering setup test",
            .API = WindowingAPICode::GLFW        ,
    };
auto window = CreateSharedPointer<MacWindow> (windowInfo);//the window is going to be partially owned by the renderer, so make a shared pointer

//now, lets create the most basic renderer, it requires a window
//you can specify optional features here, things like triangle fans, poly lines, ray tracing, ect.  (currently there are none supported
wlo::rendering::Renderer renderer(window);
 std::vector<wlo::ColorVertex3D> VertexBuffer(
            {
                    // red face
                    {.position = {-1.0f, -1.0f, 1.0f,  1.0f}, .color = {1.0f, 0.0f, 0.0f, 1.0f}},
                    {.position = {-1.0f, 1.0f,  1.0f,  1.0f}, .color = {1.0f, 0.0f, 0.0f, 1.0f}},
                    {.position = {1.0f,  -1.0f, 1.0f,  1.0f}, .color = {1.0f, 0.0f, 0.0f, 1.0f}},
                    {.position = {1.0f,  -1.0f, 1.0f,  1.0f}, .color = {1.0f, 0.0f, 0.0f, 1.0f}},
                    {.position = {-1.0f, 1.0f,  1.0f,  1.0f}, .color = {1.0f, 0.0f, 0.0f, 1.0f}},
                    {.position = {1.0f,  1.0f,  1.0f,  1.0f}, .color = {1.0f, 0.0f, 0.0f, 1.0f}},
                    // green face
                    {.position = {-1.0f, -1.0f, -1.0f, 1.0f}, .color = {0.0f, 1.0f, 0.0f, 1.0f}},
                    {.position = {1.0f,  -1.0f, -1.0f, 1.0f}, .color = {0.0f, 1.0f, 0.0f, 1.0f}},
                    {.position = {-1.0f, 1.0f,  -1.0f, 1.0f}, .color = {0.0f, 1.0f, 0.0f, 1.0f}},
                    {.position = {-1.0f, 1.0f,  -1.0f, 1.0f}, .color = {0.0f, 1.0f, 0.0f, 1.0f}},
                    {.position = {1.0f,  -1.0f, -1.0f, 1.0f}, .color = {0.0f, 1.0f, 0.0f, 1.0f}},
                    {.position = {1.0f,  1.0f,  -1.0f, 1.0f}, .color = {0.0f, 1.0f, 0.0f, 1.0f}},
                    //{ blue face
                    {.position = {-1.0f, 1.0f,  1.0f,  1.0f}, .color = {0.0f, 0.0f, 1.0f, 1.0f}},
                    {.position = {-1.0f, -1.0f, 1.0f,  1.0f}, .color = {0.0f, 0.0f, 1.0f, 1.0f}},
                    {.position = {-1.0f, 1.0f,  -1.0f, 1.0f}, .color = {0.0f, 0.0f, 1.0f, 1.0f}},
                    {.position = {-1.0f, 1.0f,  -1.0f, 1.0f}, .color = {0.0f, 0.0f, 1.0f, 1.0f}},
                    {.position = {-1.0f, -1.0f, 1.0f,  1.0f}, .color = {0.0f, 0.0f, 1.0f, 1.0f}},
                    {.position = {-1.0f, -1.0f, -1.0f, 1.0f}, .color = {0.0f, 0.0f, 1.0f, 1.0f}},
                    // yellow face
                    {.position = {1.0f,  1.0f,  1.0f,  1.0f}, .color = {1.0f, 1.0f, 0.0f, 1.0f}},
                    {.position = {1.0f,  1.0f,  -1.0f, 1.0f}, .color = {1.0f, 1.0f, 0.0f, 1.0f}},
                    {.position = {1.0f,  -1.0f, 1.0f,  1.0f}, .color = {1.0f, 1.0f, 0.0f, 1.0f}},
                    {.position = {1.0f,  -1.0f, 1.0f,  1.0f}, .color = {1.0f, 1.0f, 0.0f, 1.0f}},
                    {.position = {1.0f,  1.0f,  -1.0f, 1.0f}, .color = {1.0f, 1.0f, 0.0f, 1.0f}},
                    {.position = {1.0f,  -1.0f, -1.0f, 1.0f}, .color = {1.0f, 1.0f, 0.0f, 1.0f}},
                    //{ magenta face
                    {.position = {1.0f,  1.0f,  1.0f,  1.0f}, .color = {1.0f, 0.0f, 1.0f, 1.0f}},
                    {.position = {-1.0f, 1.0f,  1.0f,  1.0f}, .color = {1.0f, 0.0f, 1.0f, 1.0f}},
                    {.position = {1.0f,  1.0f,  -1.0f, 1.0f}, .color = {1.0f, 0.0f, 1.0f, 1.0f}},
                    {.position = {1.0f,  1.0f,  -1.0f, 1.0f}, .color = {1.0f, 0.0f, 1.0f, 1.0f}},
                    {.position = {-1.0f, 1.0f,  1.0f,  1.0f}, .color = {1.0f, 0.0f, 1.0f, 1.0f}},
                    {.position = {-1.0f, 1.0f,  -1.0f, 1.0f}, .color = {1.0f, 0.0f, 1.0f, 1.0f}},
                    //{ cyan face
                    {.position = {1.0f,  -1.0f, 1.0f,  1.0f}, .color = {0.0f, 1.0f, 1.0f, 1.0f}},
                    {.position = {1.0f,  -1.0f, -1.0f, 1.0f}, .color = {0.0f, 1.0f, 1.0f, 1.0f}},
                    {.position = {-1.0f, -1.0f, 1.0f,  1.0f}, .color = {0.0f, 1.0f, 1.0f, 1.0f}},
                    {.position = {-1.0f, -1.0f, 1.0f,  1.0f}, .color = {0.0f, 1.0f, 1.0f, 1.0f}},
                    {.position = {1.0f,  -1.0f, -1.0f, 1.0f}, .color = {0.0f, 1.0f, 1.0f, 1.0f}},
                    {.position = {-1.0f, -1.0f, -1.0f, 1.0f}, .color = {0.0f, 1.0f, 1.0f, 1.0f}},
            });

//The special sauce with this renderer is render paths. Render Paths are roughly analagous to the graphics pipeline under the hood
//they also describe where the data ends up, let's make one now

//wlo::filesystem is in the works, but for now let's just use: 

//std::filesystem::path vertexShaderRoot = std::filesystem::current_path().parent_path().append("Willow").append("shaders");
//std::filesystem::path fragmentShaderRoot = std::filesystem::current_path().parent_path().append("Willow").append("shaders");
//std::string vertShaderPath = vertexShaderRoot.append("vert.spv").string();
//std::string fragShaderPath = fragmentShaderRoot.append("frag.spv").string();


RenderPath basic{
    .camera = OrthographicCamera3D(window),//we need a camera, the simplest one just takes in the whole window
    .vertexShaderPath = "C:\\Users\\sacer\\Source\\Repos\\will1996\\Willow\\shaders\\vert.spv",//"/Users/w/Projects/Willow/shaders/vert.spv",// just give the render path file paths to the shader text files
    .fragmentShaderPath =  "C:\\Users\\sacer\\Source\\Repos\\will1996\\Willow\\shaders\\frag.spv",//"/Users/w/Projects/Willow/shaders/frag.spv",
};


//this contains all of the information necessary to build a graphics pipeline.
// The data that the RenderPath can accept is determined by the vertex shader input layout
//this renderer is highly literal, this is to give the computer as many opporutunities as possible to optimize with Vulkan

//the idea behind a renderpath is similar to a scene, it contains all of the information that the renderer needs to
//allocate vertex buffers, compile shaders, and build graphics pipelines. This high level version allows the creation
//of a library of graphics pipelines, as well as enables a guarantee that we have enough memory allocated by the time
//we want to submit frames, so no allocations during the main game loop. Renderpaths should ideally be setup when the
//game starts, you can certainly make more on the fly, but in general you shouldn't need to. For instance, in a chunk
//loading situation, you should be able to specify enough vertices in the chunk path to ensure no allocations,
//however, in the event that it amkes more sense to have a renderpath handle a single chunk, you can just use:

//VertexBuffer.attachment()*16

//the renderer needs to set up a render path before a frame can be submitted on it, this is to allow the renderer
//to validate the path, as well as create necessary resources for rendering i.e allocating buffers for the data types requested,
//creating commandbuffer pools for the operations requested, building textures, etc.
//if this setup step is skipped, the engine will throw an error on the first draw that is submitted on the path.
//Optionally, the renderer can be supplied with a list of renderpaths at instatiation, which will setup all of the paths requested.
//you can also setup a list of paths all at once by calling Setup with a list of paths.

glm::mat4x4 modelMatrx1{1};
glm::mat4x4 modelMatrx2{1};
glm::mat4x4 modelMatrx3{1};
glm::mat4x4 modelMatrx4{1};
Frame next({
                    Draw{{DataView(VertexBuffer),DataView(VertexBuffer),modelMatrx1}, basic},
                    Draw{{DataView(VertexBuffer),DataView(VertexBuffer),modelMatrx2}, basic},
                    Draw{{DataView(VertexBuffer),DataView(VertexBuffer),modelMatrx3}, basic},
                    Draw{{DataView(VertexBuffer),DataView(VertexBuffer),modelMatrx4}, basic},
          }
);

renderer.PrepareFrameClass(next);//needs to happen once per declared frame class (runtime error if not);

// a frame is a sequence of events, all of which culminate in creating a frame on the screen sometime after submission
auto start = std::chrono::high_resolution_clock::now();
while(!window->shouldClose()) {
auto delta = std::chrono::high_resolution_clock::now() -start;
    window->checkIn();
    renderer.Submit(next);
    modelMatrx1 = glm::rotate(glm::mat4(1.0f),  delta.count()*glm::radians(90.0f), glm::normalize(glm::vec3(0.0f, 1.0f, 1.0f)));
    modelMatrx2 = glm::translate(glm::mat4x4(1), sinf(delta.count())*glm::vec3(0, 3, 0));//*glm::rotate(glm::mat4(1.0f),  0.01f*delta.count()*glm::radians(90.0f), glm::normalize(glm::vec3(0.0f, 1.0f, 1.0f)));
    modelMatrx3 = glm::translate(glm::mat4x4(1), sinf(delta.count())*glm::vec3(3, 0, 0));//*glm::rotate(glm::mat4(1.0f),  0.01f*delta.count()*glm::radians(90.0f), glm::normalize(glm::vec3(0.0f, 1.0f, 1.0f)));
    modelMatrx4 = glm::translate(glm::mat4x4(1), sinf(delta.count())*glm::vec3(0, 0, 3));//*glm::rotate(glm::mat4(1.0f),  0.01f*delta.count()*glm::radians(90.0f), glm::normalize(glm::vec3(0.0f, 1.0f, 1.0f)));
}
//all data referenced by draws in the frame is copied to a the gpu by the time submit returns, so attachments are
//safe to write to immediately after submission.
};











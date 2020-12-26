//
// Created by W on 11/3/20.
//
#include "willow/rendering/Renderer.hpp"
#include "willow/window/mac_window.hpp"
#include "willow/rendering/Buffer.hpp"
using namespace  wlo::rendering;
using namespace wlo;
int main(){
    //first, let's make something to render to:
    MacWindow::Info windowInfo{
            .m_height = 800,
            .m_width = 600,
            .m_title = "rendering setup test",
            .API = WindowingAPICode::GLFW        ,
    };
auto window = CreateSharedPointer<MacWindow> (windowInfo);//the window is going to be partially owned by the renderer, so make a shared pointer

//now, lets create the most basic renderer, it requires a window
//you can specify optional features here, things like triangle fans, poly lines, ray tracing, ect.  (currently there are none supported
wlo::rendering::Renderer renderer(window);

wlo::rendering::Buffer<Vertex2D> VertexBuffer(Attachment::Type::VertexBuffer,{
        {.position = {-.5,0}},// the nested syntax on these is regrettable, but it makes sense for more complex types
        {.position = {.5,0}},
        {.position = {0,.5}}
});

wlo::rendering::Buffer<Index> IndexBuffer(Attachment::Type::IndexBuffer,{0,1,2});


//The special sauce with this renderer is render paths. Render Paths are roughly analagous to the graphics pipeline under the hood
//they also describe where the data ends up, let's make one now
auto vertShader = CreateSharedPointer<VertexShader> ("/Users/w/Projects/Willow/shaders/vert.spv");
auto fragShader = CreateSharedPointer<FragmentShader> ("/Users/w/Projects/Willow/shaders/frag.spv");

RenderPath basicIndexed{
    .attachments = {VertexBuffer.attachment(),IndexBuffer.attachment()},//attachments are typeless versions of buffers bound to the renderer, they allocate memory on the GPU
    .camera = OrthographicCamera3D(window),//we need a camera, the simplest one just takes in the whole window
    .vertexShader = vertShader,
    .fragmentShader = fragShader,
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

renderer.Setup(basicIndexed);
//the renderer needs to set up a render path before a frame can be submitted on it, this is to allow the renderer
//to validate the path, as well as create necessary resources for rendering i.e allocating buffers for the data types requested,
//creating commandbuffer pools for the operations requested, building textures, etc.
//if this setup step is skipped, the engine will throw an error on the first draw that is submitted on the path.
//Optionally, the renderer can be supplied with a list of renderpaths at instatiation, which will setup all of the paths requested.
//you can also setup a list of paths all at once by calling Setup with a list of paths.

Frame next(
          {
                    SetClearColor{ {0.0f,0.0f,0.0f,0.0f} },
                    Draw{VertexBuffer.attachment(), basicIndexed,{IndexBuffer.attachment()}},
          }
);
// a frame is a sequence of events, all of which culminate in creating a frame on the screen sometime after submission
renderer.Submit(next);//finally, draw your little triangle
//all data referenced by draws in the frame is copied to a the gpu by the time submit returns, so attachments are
//safe to write to immediately after submission.
};











//
// Created by W on 9/20/20.
//


#include "willow.hpp"
#include<iostream>
#include"glm/gtc/matrix_transform.hpp"
#include"willow/rendering/Model.hpp"
#include"willow/root/FileSystem.hpp"

class CubeExample : public wlo::Application{
private:
    const float cameraSpeed = 50.0f;
    struct {
       bool pressedForward;
        bool pressedBack;
        bool pressedLeft;
        bool pressedRight;
        bool pressedRotateLeft;
        bool pressedRotateRight;
    }inputHandler;

    wlo::rendering::Model3D cube{

            .vertices = {

                    { .position = {-1.0f, 1.0f, -1.0},.TexCoord = {0,0}},
                { .position = {1.0f, 1.0f, -1.0},.TexCoord = {1,0}},
                { .position = {1.0f, -1.0f, -1.0},.TexCoord = {1,1}},
                    { .position = {-1.0f, -1.0f, -1.0},.TexCoord = {0,1}},

                { .position = {-1.0f, 1.0f, 1.0},.TexCoord = {0,1}},
                { .position = {1.0f, 1.0f, 1.0},.TexCoord = {1,1}},
                { .position = {1.0f, -1.0f, 1.0},.TexCoord = {1,0}},
                { .position = {-1.0f, -1.0f, 1.0},.TexCoord = {0,0}},

            },

            .indices = {
                // front
                0, 1, 2,
                2, 3, 0,
                // right
                1, 5, 6,
                6, 2, 1,
                // back
                7, 6, 5,
                5, 4, 7,
                // left
                4, 0, 3,
                3, 7, 4,
                // bottom
                4, 5, 1,
                1, 0, 4,
                // top
                3, 2, 6,
                6, 7, 3
            },
            .textureFile = wlo::FileSystem::Root()+"examples\\Textures\\cow.bmp"

    };
    std::unordered_map<std::string, wlo::rendering::RenderPath> renderPaths{ };
    wlo::PrespectiveCamera3D camera;


public:
    CubeExample():Application(Application::Info("Cube example",1)),camera(m_main_window){
        m_main_window->permit<wlo::MouseButtonMessage,CubeExample,&CubeExample::handleMouse>(this);

        renderPaths.insert(
        {"basic", {
                    .camera = &camera,
                    .vertexShaderPath = wlo::FileSystem::Root()+"shaders\\vert.spv",
                    .fragmentShaderPath =  wlo::FileSystem::Root()+"shaders\\frag.spv",
            }}
        );
    }
    void handleMouse(const wlo::MouseButtonMessage& msg){

    }

    void recieve(const wlo::KeyboardMessage& msg) override{

        if(msg.content.button==wlo::Key::Code::ESCAPE) {
            WILO_CORE_INFO("Application shutting down");
            m_shutting_down = true;
        }

        if(msg.content.button==wlo::Key::Code::S) {
            WILO_INFO("GOT KEY S")
            if(msg.content.action==wlo::KeyAction::Pressed)
                inputHandler.pressedBack = true;
            if(msg.content.action==wlo::KeyAction::Released)
                inputHandler.pressedBack = false;
        }
        if(msg.content.button==wlo::Key::Code::W) {
            WILO_INFO("GOT KEY W");
            if(msg.content.action==wlo::KeyAction::Pressed)
                inputHandler.pressedForward = true;
            if(msg.content.action==wlo::KeyAction::Released)
                inputHandler.pressedForward = false;
        }
        if(msg.content.button==wlo::Key::Code::A) {
            WILO_INFO("GOT KEY W");
            if(msg.content.action==wlo::KeyAction::Pressed)
                inputHandler.pressedLeft = true;
            if(msg.content.action==wlo::KeyAction::Released)
                inputHandler.pressedLeft = false;
        }
        if(msg.content.button==wlo::Key::Code::D) {
            WILO_INFO("GOT KEY W");
            if(msg.content.action==wlo::KeyAction::Pressed)
                inputHandler.pressedRight = true;
            if(msg.content.action==wlo::KeyAction::Released)
                inputHandler.pressedRight = false;
        }
        if(msg.content.button==wlo::Key::Code::E) {
            WILO_INFO("GOT KEY W");
            if(msg.content.action==wlo::KeyAction::Pressed)
                inputHandler.pressedRotateRight = true;
            if(msg.content.action==wlo::KeyAction::Released)
                inputHandler.pressedRotateRight = false;
        }
        if(msg.content.button==wlo::Key::Code::Q) {
            WILO_INFO("GOT KEY W");
            if(msg.content.action==wlo::KeyAction::Pressed)
                inputHandler.pressedRotateLeft = true;
            if(msg.content.action==wlo::KeyAction::Released)
                inputHandler.pressedRotateLeft = false;
        }



    }


    void setup () override{
        using namespace wlo::rendering;
        Frame example = {
                Frame::Draw{cube.draw(),renderPaths["basic"]},
        };
        m_renderer->PrepareFrameClass(example);
        m_renderer->setClearColor({ {.9,.9,.9,1} });
        WILO_CORE_INFO("Rendering prepared");
    }

    void stepSim (float dt) override{
        cube.transform = glm::rotate(glm::mat4(1), timeElapsed()*1.0f,{0,1,0});
        if(inputHandler.pressedForward)
            camera.moveAlongViewAxis(dt*cameraSpeed);
        if(inputHandler.pressedBack)
            camera.moveAlongViewAxis(-dt*cameraSpeed);
        if(inputHandler.pressedLeft)
            camera.strafe(dt*cameraSpeed);
        if(inputHandler.pressedRight)
            camera.strafe(-dt*cameraSpeed);
        if(inputHandler.pressedRotateLeft)
            camera.rotate(-dt*cameraSpeed);
        if(inputHandler.pressedRotateRight)
            camera.rotate(dt*cameraSpeed);
    }

    void draw() override{
        using namespace wlo::rendering;
            Frame next = {
                    Frame::Draw{cube.draw(),renderPaths["basic"]},
            };
            m_renderer->Submit(next);
    }


   ~ CubeExample(){
        Application::reclaim();
    }

};

int main(){
    wlo::UniquePointer<wlo::Application> example = wlo::CreateUniquePointer<CubeExample>();
    example->run();
}


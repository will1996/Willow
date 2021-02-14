//
// Created by W on 9/20/20.
//
#include "willow.hpp"
#include<iostream>
#include "willow/messaging/Messages.hpp"
#include"willow/rendering/Model.hpp"
#include"willow/root/FileSystem.hpp"

class CubeExample : public wlo::Application{
private:
    const float cameraSpeed = 50.0f;
    bool cursorLockedState = true;
    struct {
       bool pressedForward;
        bool pressedBack;
        bool pressedLeft;
        bool pressedRight;
        bool pressedRotateLeft;
        bool pressedRotateRight;
    }inputHandler;
    wlo::rendering::Model<wlo::TexturedVertex3D> cube{

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
            .material{ 
            .vertexShader= wlo::FileSystem::Root().append("shaders").append("vert.spv"),
            .fragmentShader=  wlo::FileSystem::Root().append("shaders").append("frag.spv"),
            .texture = wlo::FileSystem::Root().append("examples").append("Textures").append("cow.bmp")
            },
    };

    wlo::rendering::Model<wlo::TexturedVertex3D> kitCube{
        .vertices = cube.vertices,
        .indices = cube.indices,
        .material = {
            .vertexShader = cube.material.vertexShader,
            .fragmentShader = cube.material.fragmentShader,
            .texture = wlo::FileSystem::Root().append("examples").append("Textures").append("kit.jpeg")
        }
    };

    glm::mat4x4 cubeTransform = glm::mat4x4{1};
    glm::mat4x4 kitCubeTransform = glm::mat4x4{1};
    wlo::PrespectiveCamera3D camera;


public:
    CubeExample():Application(Application::Info("Cube example",1)),camera(m_main_window){
        m_main_window->permit<wlo::MouseMoved,CubeExample,&CubeExample::handleMouse>(this);
        m_main_window->permit<wlo::MouseScrolled,CubeExample,&CubeExample::handleMouse>(this);
    }
    void handleMouse(const wlo::MouseMoved& msg){
        camera.look(msg.content.xPos,msg.content.yPos);
    }
    void handleMouse(const wlo::MouseScrolled& msg){
        camera.zoom(msg.content.yScroll_Offset);
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
        if(msg.content.button==wlo::Key::Code::TAB) {
            if(msg.content.action==wlo::KeyAction::Pressed){
                cursorLockedState  = !cursorLockedState;
                m_main_window->setCursorMode(cursorLockedState);
            }
        }
        if(msg.content.button==wlo::Key::Code::I) {
            if(msg.content.action==wlo::KeyAction::Pressed){
                cursorLockedState  = !cursorLockedState;
                m_main_window->setCursorMode(cursorLockedState);
            }
        }



    }


    void setup () override{
        using namespace wlo::rendering;
        Frame example = {
                Draw{cube,glm::mat4x4{1}},
                Draw{kitCube,glm::mat4x4{1}}
        };
        m_main_window->setCursorMode(true);
        m_renderer->setMainCamera(camera);
        m_renderer->PrepareFrameClass(example);
        m_renderer->setClearColor({ {.9,.9,.9,1} });
        WILO_CORE_INFO("Rendering prepared");
    }

    void stepSim (float dt) override{

        cubeTransform = glm::rotate(glm::mat4(1), timeElapsed()*1.0f,{0,1,0});
        kitCubeTransform = glm::translate(glm::mat4{1},glm::vec3{-5,0,0})*cubeTransform;
        if(inputHandler.pressedForward)
            camera.moveAlongViewAxis(dt*cameraSpeed);
        if(inputHandler.pressedBack)
            camera.moveAlongViewAxis(-dt*cameraSpeed);
        if(inputHandler.pressedLeft)
            camera.strafe(dt*cameraSpeed);
        if(inputHandler.pressedRight)
            camera.strafe(-dt*cameraSpeed);
        if(inputHandler.pressedRotateLeft)
            camera.roll(-dt*cameraSpeed);
        if(inputHandler.pressedRotateRight)
            camera.roll(dt*cameraSpeed);
    }

    void draw() override{
        using namespace wlo::rendering;
            Frame next = {
                    Draw{cube,cubeTransform},
                    Draw{kitCube,kitCubeTransform},
            };
            m_renderer->Submit(next);
    }


   ~ CubeExample(){
        Application::reclaim();
    }

};

int main(){
    wlo::logr::initalize();
    std::cout<<"TEST"<<std::endl;
    wlo::UniquePointer<wlo::Application> example = wlo::CreateUniquePointer<CubeExample>();
    example->run();
}


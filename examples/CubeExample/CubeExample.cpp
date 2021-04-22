//
// Created by W on 9/20/20.
//
#include "willow.hpp"
#include<iostream>
#include "willow/messaging/Messages.hpp"
#include"willow/rendering/Model.hpp"
#include"willow/root/FileSystem.hpp"
#include "willow/DefaultAssets.hpp"
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
            .mesh = wlo::assets::DefaultCube(),
            .material = {
            .vertexShader= wlo::FileSystem::Assets().append("Shaders").append("shader.vert.spv").string(),
            .fragmentShader=  wlo::FileSystem::Assets().append("Shaders").append("shader.frag.spv").string(),
            .texture = wlo::FileSystem::Assets().append("Textures").append("cow.bmp").string()
                }
    };

    wlo::rendering::Model<wlo::TexturedVertex3D> kitCube{
        .mesh = wlo::assets::DefaultCube(),
        .material = {
            .vertexShader= wlo::FileSystem::Shaders().append("shader.vert.spv").string(),
            .fragmentShader=  wlo::FileSystem::Shaders().append("shader.frag.spv").string(),
            .texture = wlo::FileSystem::Textures().append("kit.jpeg").string()
            }
    };

    wlo::PerspectiveCamera3D camera;
    wlo::rendering::Scene mainScene;
   std::map<std::string, wlo::rendering::RenderObject *> SceneObjects;


public:
    CubeExample(std::string argv_0):Application(Application::Info("Cube example",1),argv_0),camera(m_mainWindow)

    {
        m_mainWindow.permit<wlo::MouseMoved,CubeExample,&CubeExample::handleMouse>(this);
        m_mainWindow.permit<wlo::MouseScrolled,CubeExample,&CubeExample::handleMouse>(this);
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
                m_mainWindow.setCursorMode(cursorLockedState);
            }
        }
        if(msg.content.button==wlo::Key::Code::I) {
            if(msg.content.action==wlo::KeyAction::Pressed){
                cursorLockedState  = !cursorLockedState;
                m_mainWindow.setCursorMode(cursorLockedState);
            }
        }



    }


    void setup () override{
        using namespace wlo::rendering;
        m_renderer.preAllocateScene({
           .vertexCounts = {{wlo::data::Type::of<wlo::TexturedVertex3D>(),16}} ,
           .materials = {&cube.material,&kitCube.material},
          .totalIndexCount = 72
        });

        m_mainWindow.setCursorMode(true);
        m_renderer.setMainCamera(camera);
        m_renderer.setClearColor({ {.9,.9,.9,1} });
        WILO_CORE_INFO("Rendering prepared");
    }

    void stepSim (float dt) override{
        //for(auto [name,pObject]:SceneObjects)
            //pObject->transform = glm::rotate(glm::mat4(1), timeElapsed()*1.0f,{0,1,0});

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
        Frame next{
            Draw{cube,glm::mat4x4{1}},
            Draw{kitCube,glm::translate(glm::mat4x4{1},{0,2,0})}
        };
        m_renderer.submit(next);
    }


   ~ CubeExample() override{
        Application::reclaim();
    }

};

int main(int argc, char ** argv){
    wlo::logr::initalize();
    wlo::UniquePointer<wlo::Application> example = wlo::CreateUniquePointer<CubeExample>(argv[0]);
    example->run();
}


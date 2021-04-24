//
// Created by W on 9/20/20.
//
#include "willow.hpp"
#include<iostream>
#include "willow/messaging/Messages.hpp"
#include"willow/root/FileSystem.hpp"
#include "willow/DefaultAssets.hpp"
#include "willow/rendering/Mesh.hpp"
class CubeExample : public wlo::Application{
private:
    const float cameraSpeed = 50.0f;
    bool cursorLockedState = true;


    wlo::Mesh cubeMesh = wlo::assets::DefaultCube();
    wlo::PerspectiveCamera3D camera;
    wlo::rendering::Scene mainScene;
    wlo::TextureHandle cowTexture;
    wlo::TextureHandle kitTexture;
    wlo::MaterialHandle cowMaterial;
    wlo::MaterialHandle kitMaterial;


public:
    CubeExample(std::string argv_0) :
        Application(Application::Info("Cube example", 1), argv_0),
        camera(m_mainWindow),
        cowTexture(m_assets.loadTexture(wlo::FileSystem::Texture("cow.bmp").string())),
        cowMaterial(m_assets.loadMaterial(cowTexture.id, wlo::FileSystem::Shader("shader.vert").string(), wlo::FileSystem::Shader("shader.frag").string())),
        kitTexture(m_assets.loadTexture(wlo::FileSystem::Texture("kit.jpeg").string())),
        kitMaterial(m_assets.loadMaterial(kitTexture.id, wlo::FileSystem::Shader("shader.vert").string(), wlo::FileSystem::Shader("shader.frag").string()))
    {
        m_mainWindow.permit<wlo::MouseMoved,CubeExample,&CubeExample::handleMouse>(this);
        m_mainWindow.permit<wlo::MouseScrolled,CubeExample,&CubeExample::handleMouse>(this);


        m_input.setKeyMap({ 
            {"Forward",wlo::Key::Code::W},
            {"Backward",wlo::Key::Code::S},
            {"Left",wlo::Key::Code::A},
            {"Right",wlo::Key::Code::D},
                                });

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
        mainScene
            .add(cubeMesh, cowMaterial.get(), glm::translate(wlo::Mat4{ 1 }, wlo::Vec3{ 0,0,2 }))
            .add(cubeMesh, kitMaterial.get(), wlo::Mat4{ 1 });

        m_renderer.preAllocateScene(mainScene.getDescription());


        //m_renderer.preAllocateScene({
        //   .vertexCounts = {{wlo::data::Type::of<wlo::TexturedVertex3D>(),16}} ,
        //   .materials = {&cowMaterial.get(), &kitMaterial.get() },
        //  .totalIndexCount = 72
        //});

        m_mainWindow.setCursorMode(true);
        m_renderer.setMainCamera(camera);
        m_renderer.setClearColor({ {.8,.8,.8,1} });



        WILO_CORE_INFO("Rendering prepared");
    }

    void stepSim (float dt) override{
        //for(auto [name,pObject]:SceneObjects)
            //pObject->transform = glm::rotate(glm::mat4(1), timeElapsed()*1.0f,{0,1,0});

        if(m_input.isPressed("Forward"))
            camera.moveAlongViewAxis(dt*cameraSpeed);
        if(m_input.isPressed("Backward"))
            camera.moveAlongViewAxis(-dt*cameraSpeed);
        if(m_input.isPressed("Left"))
            camera.strafe(dt*cameraSpeed);
        if(m_input.isPressed("Right"))
            camera.strafe(-dt*cameraSpeed);
    }
    void draw() override{
        m_renderer.render(mainScene);
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


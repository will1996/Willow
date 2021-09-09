//
// Created by W on 9/20/20.
//
#include "willow.hpp"
#include<iostream>
#include "willow/messaging/Messages.hpp"
#include"willow/root/FileSystem.hpp"
#include "willow/DefaultAssets.hpp"
#include "willow/rendering/Mesh.hpp"
#include"willow/root/Root.hpp"
#include"willow/ecs/EntityComponentSystem.hpp"
#include"willow/window/Window.hpp"
#include"willow/input/InputManager.hpp"
#include"willow/rendering/PerspectiveCamera3D.hpp"
#include"willow/rendering/Renderer.hpp"
#include"willow/ecs/Systems.hpp"
#include "willow/rendering/Scene.hpp"
#include "willow/Assets.hpp"


using namespace  wlo;
using namespace  wlo::data;
using rendering::Scene;

struct RigidBody{
    wlo::Vec3 velocity;
    wlo::Vec3 acceleration;
};

struct Transform{
    wlo::Vec3 position;
};
struct Gravity{
    wlo::Vec3 vector;
};

struct PerspectiveCamera{
    wlo::Vec3 up;
    wlo::Vec3 focus;
    wlo::Vec2 aspect;
};

namespace wlo::data{
    RegTwoMemberType(RigidBody,"velocity",Vec3,"acceleration",Vec3);
    RegOneMemberType(Transform,"position",Vec3);
    RegOneMemberType(Gravity,"vector",Vec3);
    RegThreeMemberType(PerspectiveCamera,"up",Vec3,"focus",Vec3,"aspect",Vec2);
}


System PhysicsIntegrate{
    .settings = Value(Type("settings",{{"dt",typeOf<float>()}}))
};

System Render{

};


class CubeExample:public Messenger{
public:
    UniquePointer<EntityComponentSystem> ecs;
    UniquePointer<Window> window;
    UniquePointer<InputManager> input;
    rendering::Renderer renderer;
    PerspectiveCamera3D camera;
    Systems systems;
    Assets assets;
    bool m_shouldStop = false;
    MeshHandle cubeMesh  = assets.TexturedCube();
    TextureHandle kitTexture = assets.loadTexture(wlo::FileSystem::Texture("kit.jpeg"));
    TextureHandle cowTexture = assets.loadTexture(wlo::FileSystem::Texture("cow.bmp"));
    MaterialHandle cubeMaterial0  = assets.loadMaterial(kitTexture.id,wlo::FileSystem::Shader("shader.vert"),wlo::FileSystem::Shader("shader.frag"));
    MaterialHandle cubeMaterial1  = assets.loadMaterial(cowTexture.id,wlo::FileSystem::Shader("shader.vert"),wlo::FileSystem::Shader("shader.frag"));
    CubeExample() {
        window = CreateUniquePointer<Window>(Window::Info{});
        input = CreateUniquePointer<InputManager>();
        ecs = CreateUniquePointer<EntityComponentSystem>();
        window->permit<WindowClosed,CubeExample,&CubeExample::shutdown>(this);
        input->connect(window.get());
        ecs->registerComponent<RigidBody>();
        ecs->registerComponent<Transform>();
        ecs->registerComponent<Gravity>();
        renderer.connect(window.get());
        camera.setPosition({0.0, 0.0, -5.0});
        renderer.setClearColor({0.85, 0.85, 0.85, 1.0});


        //initialization
        Scene scene;
        for (size_t i =0; i < 30; i++){
            for (size_t j =0; j < 20; j++) {
                Transform transform{.position = {j , i+j*j, i }};
                Gravity gravity{.vector = {0, -1, 0}};
                RigidBody rigidBody{.velocity{0, 0, 0}, .acceleration{0, 0, 0}};
                auto ent = ecs->spawnEntity(Value({transform, gravity, rigidBody}));
                if (ent%2==0)
                scene.add(cubeMesh.get(), cubeMaterial0.get(),
                          glm::translate(glm::mat4(1.0), ecs->getComponent<Transform>(ent).position));
                else
                    scene.add(cubeMesh.get(), cubeMaterial1.get(),
                              glm::translate(glm::mat4(1.0), ecs->getComponent<Transform>(ent).position));
            }
        }


        renderer.preAllocateScene(scene.getDescription());



    }

    void shutdown(const WindowClosed & ){
        m_shouldStop = true;
    }

    void updatePhysics(){
        const float dt = 0.0005;
        auto ents = ecs->getEntitiesOfArchetype<RigidBody,Transform,Gravity>();
        for (auto ent : ents){
            auto&  rb = ecs->getComponent<RigidBody>(ent);
            auto&  tform = ecs->getComponent<Transform>(ent);
            auto&  gravity = ecs->getComponent<Gravity>(ent);
            rb.acceleration +=gravity.vector;
            rb.velocity +=rb.acceleration*dt;
            tform.position +=rb.velocity;
        }
    }

    void draw(){
        auto ents = ecs->getEntitiesOfArchetype<Transform>();
        Scene scene;
        for (auto ent : ents){
            cout<<"adding ent: "<<ent<<"with  position"<<glm::to_string(ecs->getComponent<Transform>(ent).position)<<endl;
            if (ent%2==0)
            scene.add(cubeMesh.get(),cubeMaterial0.get(),glm::translate(glm::mat4(1.0),ecs->getComponent<Transform>(ent).position));
            else
            scene.add(cubeMesh.get(),cubeMaterial1.get(),glm::translate(glm::mat4(1.0),ecs->getComponent<Transform>(ent).position));
        }
        auto view = glm::lookAt(wlo::Vec3{1.0f,10.0f,3.0f},{0.0f,-2.0f,0.0f},{0.0f,1.0f,0.0f});
        float aspect = window.get()->getInfo().m_width/window.get()->getInfo().m_height;
        auto proj  = glm::perspective(90.0f,aspect,0.1f,100.0f);
        proj[1][1]*=-1;
        renderer.render(scene,proj*view);
    }

    void Run( ){
        while(! window->shouldClose()){
           updatePhysics();
            draw();
        }
    }
};


int main(int argc, char ** argv){
    CubeExample example;
    example.Run();
}


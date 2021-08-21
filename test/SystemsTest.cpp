//
// Created by Will Chambers on 8/18/21.
//
#include"willow/ecs/Systems.hpp"
#include"willow/data/Data.hpp"
#include"TestingTools.hpp"
#include<iostream>
using namespace wlo;
struct Transform{
    wlo::Vec3 position;
    wlo::Vec3 rotation;
};
struct RigidBody{
    wlo::Vec3 velocity;
    wlo::Vec3 acceleration;
};

template<>
data::Type data::typeOf<Transform>() {
    return data::Type("Transform",
                      {{"position",data::typeOf<Vec3>()},
                       {"rotation",data::typeOf<Vec3>()},
                      }
    );
}

template<>
data::Type data::typeOf<RigidBody>() {
   return  data::Type("RigidBody",
               {{"velocity",data::typeOf<Vec3>()},
                {"acceleration",data::typeOf<Vec3>()},
               }
    );
}
int main(){

    wlo::EntityComponentSystem ecs;
    ecs.registerComponent<RigidBody>();
    ecs.registerComponent<Transform>();
    wlo::Systems systems;
    System physicsUpdate{
        .settings = data::Value(data::Type("settings",{{"dt",data::typeOf<float>()}}))
    };
    physicsUpdate.settings["dt"] = 0.005f;
    physicsUpdate.fn = [&physicsUpdate](EntityComponentSystem& ecs){
        auto dt = physicsUpdate.settings["dt"].get<float>();
        auto ents = ecs.getEntitiesOfArchetype<Transform,RigidBody>();
        for (auto ent : ents){
            auto & rb = ecs.getComponent<RigidBody>(ent);
            auto & tf = ecs.getComponent<Transform>(ent);
            rb.velocity += rb.acceleration*dt;
            tf.position += rb.velocity*dt;
            std::cout<<tf.position.x<<std::endl;
            std::cout<<ecs.getComponent<Transform>(ent).position.x<<std::endl;
        }
    };
    systems.registerSystem("PhysicsIntegrate",physicsUpdate);
    auto ent = ecs.spawnEntity({Transform{.position = {0,0,0}},
                     RigidBody{.velocity{1.0f,0.0f,0.0f}}});

    systems.runActiveSystems(ecs);
    auto tf = ecs.getComponent<Transform>(ent);

}

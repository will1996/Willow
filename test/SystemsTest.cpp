//
// Created by Will Chambers on 8/18/21.
//
#include"willow/ecs/Systems.hpp"
#include"willow/data/Data.hpp"
#include"TestingTools.hpp"
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
    ecs.spawnEntity({Transform{.position = {0,0,0}},
                     RigidBody{}});





}

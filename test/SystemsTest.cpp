//
// Created by Will Chambers on 8/18/21.
//
#include"willow/ecs/Systems.hpp"
#include"willow/data/Data.hpp"
#include"TestingTools.hpp"

namespace wlo{
struct Transform{
    wlo::Vec3 position;
    wlo::Vec3 rotation;
};
struct RigidBody{
    wlo::Vec3 velocity;
    wlo::Vec3 acceleration;
};


int main(){
    Data::registerType<Transform>(data::Type("Transform",
                                             {{"position",Data::type<Vec3>()},
                                              {"rotation",Data::type<Vec3>()},
                                              }
    ));
    Data::registerType<Transform>(data::Type("RigidBody",
                                             {{"velocity",Data::type<Vec3>()},
                                              {"acceleration",Data::type<Vec3>()},
                                             }
    ));

    wlo::EntityComponentSystem ecs;
    wlo::Systems systems;
    ecs.spawnEntity({Transform{.position = {0,0,0}},
                     RigidBody{}});





}
}
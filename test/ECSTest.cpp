//
// Created by Will Chambers on 8/16/21.
//


#include"willow/ecs/EntityComponentSystem.hpp"
#include "TestingTools.hpp"
using namespace wlo;






int main(){
    EntityComponentSystem ecs;
    ecs.registerComponent<wlo::Vec3>();
    ecs.registerComponent<wlo::Vec2>();
    ecs.registerComponent<wlo::Vec4>();
   auto vec3 =  ecs.spawnEntity(wlo::Vec3(0,0,0));
   auto vec2 =  ecs.spawnEntity(wlo::Vec2(0,0));
   auto vec5 =  ecs.spawnEntity(wlo::data::Value({wlo::Vec2(1,2),wlo::Vec3(1,2,3)}));
   auto vec4 = ecs.spawnEntity(wlo::Vec4(0,0,0,0));

   require(ecs.componentCount<wlo::Vec2>()==2);
   require(ecs.componentCount<wlo::Vec3>()==2);
   require(ecs.componentCount<wlo::Vec4>()==1);

    ecs.removeComponent(vec3,data::typeOf<wlo::Vec3>());
    require(ecs.archetypeOf(vec3)==0);
    require(ecs.componentCount<wlo::Vec3>()==1);
    ecs.addComponent(vec3,wlo::Vec3(1,2,3));
    require(ecs.archetypeOf(vec3)==ecs.archetypeOf(wlo::Vec3()));
    require(ecs.componentCount<wlo::Vec3>()==2);
    ecs.addComponent(vec3,wlo::Vec2(1,2));
    require(ecs.archetypeOf(vec3)==ecs.archetypeOf(wlo::data::Value({wlo::Vec3(),wlo::Vec2()})));
    require(ecs.componentCount<wlo::Vec2>()==3);

    auto ents = ecs.getEntitiesOfArchetype<Vec3>();
    require(ents.size()==2);

    require(std::find(ents.begin(),ents.end(),vec3)!=ents.end());

    auto writeMe = ecs.spawnEntity(wlo::data::Value({wlo::Vec2(1,2),wlo::Vec3(0,1,2)}));
    cout<<"Writing entity"<<writeMe<<endl;
    ecs.writeEntity(writeMe,Vec3{1.0f,2.0f,3.0f});
    require(ecs.getComponent<Vec3>(writeMe)==Vec3(1.0f,2.0f,3.0f));



}
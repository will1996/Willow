//
// Created by Will Chambers on 8/17/21.
//


#include"willow/ecs/Systems.hpp"


namespace wlo{

   void Systems::registerSystem(std::string name, System sys) {
       m_systems[name] = std::move(sys);
   }

   void Systems::runActiveSystems(EntityComponentSystem &ecs) {
       for (auto psys :m_activeSystems){
           psys->fn(ecs);
       }
   }

    void Systems::setGlobal(const data::Value & v) {
       m_globals = v;
    }

}


//
// Created by Will Chambers on 8/17/21.
//

#ifndef WILLOW_SYSTEM_HPP
#define WILLOW_SYSTEM_HPP

#include"EntityComponentSystem.hpp"
namespace wlo {
    struct System {
        data::Value settings;
        EngineComponent *EngineResource;
        std::function<void(EntityComponentSystem & ecs)> fn;
    };

    class Systems : public Messenger{
    public:
        void setGlobal(const data::Value & );
        void registerSystem(std::string name, System sys);
        void runActiveSystems(EntityComponentSystem& ecs);
    private:
        data::Value m_globals;
        std::vector<System*> m_activeSystems;
        std::map<std::string, System> m_systems;
        std::map<std::string, size_t> m_activeSystemIndices;
    };
}


#endif //WILLOW_SYSTEM_HPP

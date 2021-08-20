//
// Created by Will Chambers on 8/12/21.
//

#ifndef WILLOW_ENTITYCOMPONENTSYSTEM_HPP
#define WILLOW_ENTITYCOMPONENTSYSTEM_HPP
#include "willow/root/EngineComponent.hpp"
#include <queue>
#include<unordered_map>
#include<functional>
#include"willow/data/Value.hpp"
namespace wlo{
    using Entity = uint32_t;
    using ComponentTag = std::bitset<32>;
    using Archetype = std::bitset<32>;

    struct WriteEntity{
        Entity ent;
        data::Value v;
    };

    class EntityComponentSystem : public EngineComponent{
    public:

        template<typename ...Ts>
        Entity spawnEntity(){
          Entity ent = entityCounter++;
           (assignComponent<Ts>(ent),...);
           std::cout<<archetypeOf(ent)<<std::endl;
        };

        /*spawning an entity from a Value.
         *
         * Values are recursive, so it's impossible to know what to expect in general.
         *
         * However, I've chosen here to accept the two most common use cases.
         * 1: Initializing a single component with a single Value.
         * The Component must be registered, and the typename of the Value must match the registered name.
         * 2: Initialzing an entire archetype from one Value.
         * To Do this, each member of the Value must be a registered component.
         * */
        Entity spawnEntity(data::Value v){
            Entity ent = entityCounter++;
            data::Type t = v.getType();
             auto maintag = m_componentTagsByName[v.getType().name()];
             if (maintag!=0){
                 addComponent(ent,v);
             }
             else {
                 for (auto const &member : t.getMembers()) {
                     addComponent(ent,v[member.name]);
                 }
             }
            return ent;
        }

        Entity spawnEntity(std::vector<data::Value> aggV){
            spawnEntity(wlo::data::Value(aggV));
        }


        void addComponent(Entity ent,data::Value v){
            data::Type t = v.getType();
            auto tag = m_componentTagsByName[v.getType().name()];
            assert(tag != 0 && "Must initialize entities from registered types");
            m_componentAssigners[tag](ent);
            m_componentWriters[tag](ent,v);
        }

        void removeComponent(Entity ent, data::Type t){
            auto tag = m_componentTagsByName[t.name()];
            m_componentRemovers[tag](ent);
        }

        inline Archetype archetypeOf(Entity ent)
        {
            return entityArchetypes[ent];
        }

        template<typename T>
        size_t componentCount(){
            return componentStorage<T>.size();
        }

        inline Archetype archetypeOf(const data::Value & v)
        {
            auto tag = m_componentTagsByName[v.getType().name()];
            if(tag==0) {
                for (auto member : v.getType().getMembers()){
                    auto memberTag = m_componentTagsByName[member.type->name()];
                    assert(memberTag!=0&&"Value is not itself a component, and has non component members. Therefore has no archetype");
                    tag|=memberTag;
                }
            }
            return tag;
        }

        inline void writeEntity(const WriteEntity & msg){
            writeEntity(msg.ent,msg.v);
        }

        inline void writeEntity(Entity e,data::Value v){

            ComponentTag tag = m_componentTagsByName[v.getType().name()];
            if (tag == 0 ) {
                std::cout << "Attempted to write entity " << e << "with unregistered component type: "
                          << v.getType().name() << std::endl;
                return;
            }else if ((tag&archetypeOf(e))==0){
                std::cout << "Attempted to write entity " << e << "with component type: "
                          << v.getType().name() << "Which is not assigned to ent: "<<e<<std::endl;
                return;

            }
            std::cout<<"Fetched tag: "<<tag<<"for component of name "<<v.getType().name()<<std::endl;
            m_componentWriters[tag](e,v);
        }

        template<typename T>
        void registerComponent(){
            assert(componentTags<T> ==0&& "Cannot register components  more than once, this, unfortunately, means typedefs too");
            componentStorage < T >.resize(0);
            assert(componentCounter < 32 && "Too many component types registered");
            componentTags < T > = ComponentTag(std::pow(2, componentCounter));
            m_componentTagsByName[Data::type<T>().name()] = componentTags<T>;
            m_componentWriters[componentTags < T > ] = [](Entity ent, const data::Value &v) {
                componentStorage<T>[componentIndices<T>[ent]] = v.get<T>();
            };
            m_componentAssigners[componentTags < T > ] = [=](Entity ent) {
                assignComponent<T>(ent);
            };
            m_componentRemovers[componentTags < T > ] = [=](Entity ent) {
                removeComponent<T>(ent);
            };
            componentCounter++;
        }

        template<typename ...Ts>
        std::vector<Entity> getEntitiesOfArchetype(){
            Archetype archetypeToFind = buildArchetype<Ts...>();
            std::cout<<"Searching for Archetype "<<archetypeToFind<<std::endl;
            std::vector<Entity> entities;
            for (auto &[entity,archetype] : entityArchetypes){
                if (archetype==archetypeToFind)
                    entities.push_back(entity);
            }
            return entities;
        }

        template<typename T>
        T& getComponent(Entity ent){
            return componentStorage<T>[componentIndices<T>[ent]];
        }



    private:
        template<typename ...Ts>
        Archetype buildArchetype(){
            Archetype baseTag;
            (appendTag<Ts>(baseTag),...);
            return baseTag;
        }

        template<typename T>
        ComponentTag appendTag(Archetype & arch){
            assert(componentTags<T>!=0 && "Component does not exits");
            arch|= componentTags<T>;
            return arch;
        }


        template<typename T>
        void assignComponent(Entity ent){
            if(componentTags<T> ==0)
                registerComponent<T>();

            entityArchetypes[ent]|=componentTags<T>;
            componentIndices<T>[ent] = componentStorage<T>.size();//assign current end+1 index to this ent
            componentStorage<T>.resize(componentStorage<T>.size()+1);//grow container to make end+1 end
        }

        /*
         * First, extract the index of the component we're removing.
         * then, find the entity at the end of this component list
         * Next, move the last component in this compinent list to that index
         * assign the moved to index to the entity formerly at the end of the component list
         * finally, xor the entity signature with the component tag for the removed component, to remove it from
         * that entities archetype
         * */
        template<typename T>
        void removeComponent(Entity ent){
            assert(componentTags<T> != 0&&"Attempting to remove an unregistered component, Nonsensical");
            size_t freedIndex = componentIndices<T>[ent];
            size_t backIndex = componentStorage<T>.size()-1;
            size_t occupyingEnt;
            for (auto & [key,value] : componentIndices<T>){
                if (value==occupyingEnt)
                    occupyingEnt = key;
            }
            componentStorage<T>[freedIndex]  = componentStorage<T>[backIndex];
            componentIndices<T>[occupyingEnt] = freedIndex;
            componentIndices<T>.erase(ent);
            componentStorage<T>.resize(componentStorage<T>.size()-1);
            entityArchetypes[ent]^=componentTags<T>;
        }
        std::unordered_map<ComponentTag, std::function<void(Entity ent,const data::Value&)> > m_componentWriters;
        std::unordered_map<ComponentTag, std::function<void(Entity ent)> > m_componentAssigners;
        std::unordered_map<ComponentTag, std::function<void(Entity ent)> > m_componentRemovers;
        std::unordered_map<std::string, ComponentTag > m_componentTagsByName ;

        template<typename T>
         static   std::vector<T> componentStorage;

        template<typename T>
        static   std::unordered_map<Entity,size_t> componentIndices;


        template<typename T>
        static ComponentTag componentTags;

        size_t componentCounter =0;
        size_t entityCounter =0;

        std::unordered_map<Entity,ComponentTag> entityArchetypes;



    };

    //allocate memory for static component storage
    template<typename T>
    std::vector<T> EntityComponentSystem::componentStorage;
    template<typename T>
    ComponentTag EntityComponentSystem::componentTags;
    template<typename T>
    std::unordered_map<Entity,size_t> EntityComponentSystem::componentIndices;
}



#endif //WILLOW_ENTITYCOMPONENTSYSTEM_HPP

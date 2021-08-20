//
// Created by Will Chambers on 8/11/21.
//

#include"willow/root/Core.hpp"

#include "willow/messaging/Messages.hpp"

#include"willow/root/EngineComponent.hpp"
namespace wlo{

   void Core::mainLoop(){
       notify(EngineStart{}); //synchronously calls startup methods of all attached engine components

       while (m_isRunning){
           notify(FrameStart{});
           notify(ReadInput{});
           notify(RenderStart{});
       }
        notify(EngineStop{});
   }


   void Core::stop(){
       m_isRunning = false;
   }

   void Core::onShutdown(const Shutdown &) {
       stop();
   }

    Core::Core(vector<UniquePointer<EngineComponent>> components):
    m_components(std::move(components))
    {
        //connect all components to eachother, allowing each component to decide what it's interested in
       for( size_t i =0; i < m_components.size(); i++){
           m_components[i]->connect(this);
           for( size_t j =0; j < m_components.size(); j++){
               if(i!=j){
                    m_components[i]->connect(m_components[j].get());
               }
           }
        }
    }

    void Core::addComponent(UniquePointer<EngineComponent> comp) {
       comp->connect(this);
       for (auto  &component : m_components){
            comp->connect(component.get());
            component->connect(comp.get());
       }
       m_components.push_back(std::move(comp));

    }


}
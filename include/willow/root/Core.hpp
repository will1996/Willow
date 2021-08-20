//
// Created by Will Chambers on 8/11/21.
//

#ifndef WILLOW_CORE_HPP
#define WILLOW_CORE_HPP
#include"willow/messaging/MessageSystem.hpp"
#include"willow/root/EngineComponent.hpp"


namespace wlo{


   class Core:public Messenger {
   public:
       Core(std::vector<UniquePointer<EngineComponent>>);
       void addComponent(UniquePointer<EngineComponent> comp);
       void mainLoop();
       void stop();
       void onShutdown(const Shutdown & );
   private:
       bool m_isRunning = true;
       std::vector<UniquePointer<EngineComponent>> m_components;

   };



}


#endif //WILLOW_CORE_HPP

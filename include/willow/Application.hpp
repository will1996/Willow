#pragma once
#include"willow/root/Root.hpp"
#include"willow/root/Core.hpp"
#include"willow/root/EngineComponent.hpp"
#include<unordered_map>
namespace wlo{
    /*

    */
class Application: public Messenger{
        public:
            Application(std::string argv_0);//Application must recieve argv[0] in its constructor, it uses this path to
            void run();
            ~Application();
            void stop(const WindowClosed &);
        protected:
            decltype(std::chrono::high_resolution_clock::now()) startTime;
            float timeElapsed();
            std::unordered_map<std::string,UniquePointer<EngineComponent> >m_engineComponents;
            Core m_core;

    };
    
}

#pragma once
#include<memory>

#include "EngineComponent.hpp"
#include"willow/messaging/Messages.hpp"
#include<sstream>
namespace wlo{

    struct LogContent{
        std::string text;
        template<typename ...Args>
        explicit LogContent(Args ...args){
            std::stringstream ss;
            (ss <<...<< std::forward(args));
            text = ss.str();
        }

        LogContent(std::string && str):text(str){}
        LogContent(const char *  str):text(str){}

        LogContent & operator = (LogContent && other){
            text = other.text;
            return *this;
        }


    };



    class Logger : public EngineComponent{
    public:
        struct Info :Message{
            LogContent content;
        };

        Logger() = default;

        void connect(Messenger * comp) override{
            comp->permit<Logger::Info, Logger, &Logger::receive>(this);
        }

        inline void receive(const  Info & msg){
            std::cout<<"Log[Info]: "  <<msg.content.text<<std::endl;
        }



    };

}

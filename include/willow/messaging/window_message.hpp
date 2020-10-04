#pragma once
#include"wilo_message.hpp"
#include"willow/root/wilo_dev_core.hpp"
namespace wlo{
    class WindowMessage : public Message{
            public:
                struct Info  {
                    std::string title;
                    double width;
                    double height;
                };
                WindowMessage(MessageType type, WindowMessage::Info info);
                WindowMessage::Info getInfo() const ;
                std::string toString() const override;
                MessageType getType() const override;
            private:
                WindowMessage::Info m_info;
                MessageType m_type;
                std::string m_name;
    };
}
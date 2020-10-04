#include"willow/messaging/keyboard_message.hpp"
#include"willow/messaging/wilo_message.hpp"
#include"willow/root/wilo_dev_core.hpp"
#include<functional>

namespace wlo{
    KeyboardMessage::KeyboardMessage(MessageType type, KeyboardMessage::Info info):
    m_type(type),
    m_info(info)
    {
            std::stringstream namestream;
            switch(type){
                case MessageType::KeyPressed:
                namestream << "KeyPressed Message, Button: " <<info.button;
                namestream<<"repeated: "<<info.repeat_length<<" times ";
                break;
                case MessageType::KeyReleased:
                namestream << "KeyReleased Message, Button: " <<info.button;
                break;
                case MessageType::KeyHeld:
                namestream << "KeyHeld Message, Button: " <<info.button;
                break;
                default:
                    std::stringstream ss;
                    ss<<"invalid message type: "<<NameOfMessageType[type]<< "for KeyboardMessage";
                    throw std::runtime_error(ss.str());
            }
                namestream <<"Modifiers: "<< info.unpackModBundleName();
                m_name = namestream.str();
                //WILO_CORE_INFO("keyboard event created with name value {0}",m_name);
    }

    MessageType KeyboardMessage::getType()  const {
        return m_type;
    }
    std::string KeyboardMessage::toString() const{
        return m_name;
    }
    KeyboardMessage::Info  KeyboardMessage::getInfo() const {
        return m_info;
    }
}

#include"willow/messaging/mouse_message.hpp"

namespace wlo{
    MouseMessage::MouseMessage(MessageType type, MouseMessage::Info info):
    m_type(type),
    m_info(info)
    {
        std::stringstream ss;
        switch(type){

            case MessageType::MouseButtonPressed:
                if( ! info.button.has_value()) 
                    throw std::runtime_error("must set button value for MouseButtonPressed signals");
                ss<<"MouseButtonPressed: ";
                ss<<info.button.value();
                break;
            
            case MessageType::MouseButtonReleased:
                if( ! info.button.has_value()) 
                    throw std::runtime_error("must set button value for MouseButtonReleased signals");
                ss<<"MouseButtonReleased: ";
                ss<<info.button.value();
                break;

            case MessageType::MouseMoved:
                if( ! info.xPos.has_value() && info.yPos.has_value()) 
                    throw std::runtime_error("must set xpos and ypos value for MouseMoved signals");
                ss<<"MouseMoved, New position: ";
                ss<<"x: "<<info.xPos.value();
                ss<<"y: "<<info.yPos.value();
                break;
            
            case MessageType::MouseScrolled:
                if( ! (info.xScroll_Offset.has_value() || info.yScroll_Offset.has_value())) 
                    throw std::runtime_error("must set xScroll_offset or yScroll_offset value for MouseScrolled messages");
                ss<<"MouseScrolled: ";
                ss<<"x_offset: "<<info.xScroll_Offset.value();
                ss<<"y_offset: "<<info.yScroll_Offset.value();
                break;

            default :
                throw std::runtime_error("invalid signal type for Mouse signal");
                break;

        }
            ss<<"Modifier "<<info.unpackModBundleName();
            m_name = ss.str();
    }
   MouseMessage::Info MouseMessage::getInfo(){
        return m_info;
       } 
    std::string MouseMessage::toString() const{
        return m_name;
    }
    MessageType MouseMessage::getType() const{
        return m_type;
    }

}
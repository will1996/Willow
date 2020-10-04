#include "willow/root/wilo_dev_core.hpp"
#include "willow/messaging/window_message.hpp"
namespace wlo{
                WindowMessage::WindowMessage(MessageType type, WindowMessage::Info info):
                m_type(type),
                m_info(info)
                {
                    std::stringstream namestream;
                        namestream<<"Signal:";
                    switch(type){
                        case MessageType::WindowClose:
                            namestream<<"WindowClose";
                            break;
                        case MessageType::WindowResized:
                            namestream<<"WindowResized: ";
                            namestream<<"new width: "<<info.width;
                            namestream<<"new height: "<<info.height;
                            break;
                        case MessageType::WindowLoseFocus:
                            namestream<<"WindowLoseFocus";
                            break;
                        case MessageType::WindowGainFocus:
                            namestream<<"WindowGainFocus";
                            break;
                        default:
                            std::string msg = "Unsupproted SignalType: " + NameOfMessageType[type]+" for SignalClass WindowSignal";
                            throw std::runtime_error(msg);
                        break;
                    }
                        m_name = namestream.str();
                }
                WindowMessage::Info  WindowMessage::getInfo() const {
                    return m_info;
                }
                std::string WindowMessage::toString() const {
                    return m_name;
                }

                MessageType  WindowMessage::getType() const {
                    return m_type;
                }
}
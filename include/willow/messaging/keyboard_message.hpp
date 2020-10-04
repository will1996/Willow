#pragma once

#include "wilo_message.hpp"
#include "willow/input/KeyCodes.hpp"
#include "willow/root/wilo_dev_core.hpp"
namespace wlo{
class KeyboardMessage : public Message{
    public:
        struct Info{
            KeyCode button;
            uint16_t mod_bundle = 0 ;
            uint32_t repeat_length = 0;
            bool hasModifier(KeyModifier mod){
                return (mod_bundle|mod)!=0;
            }
            std::string unpackModBundleName(){
                std::stringstream name;
                name<<"|";
                if (mod_bundle&KeyModifier::ALT)
                    name<<"ALT|";
                if (mod_bundle&KeyModifier::SHIFT)
                    name<<"SHIFT|";
                if (mod_bundle&KeyModifier::SUPER)
                    name<<"SUPER|";
                return name.str();
            }
        };
        KeyboardMessage(MessageType type, Info info );
        MessageType getType() const override;
        std::string toString() const override;
        Info getInfo() const;
    private:
        std::string m_name;
        MessageType m_type;
        Info m_info;
};
}

//
// Created by Will Chambers on 3/2/21.
//

#include"willow/input/InputManager.hpp"
#include"willow/root/Logger.hpp"
#include<sstream>
namespace wlo{
   InputManager::InputManager() {
   }

   void InputManager::recieveKey(const KeyboardMessage & msg) {
       if(msg.content.action==wlo::KeyAction::Pressed)
        m_pressedMap[msg.content.button] = true;
       if(msg.content.action==wlo::KeyAction::Released)
        m_pressedMap[msg.content.button] = false;
   }


    void InputManager::resetPressedMap() {

    }

    bool InputManager::isPressed(wlo::Key::Code code) const {
        if(! m_pressedMap.contains(code))
            return false;
        return m_pressedMap.at(code);
    }

    bool InputManager::isPressed(std::string keyName) const{
       if(! m_keyMap.contains(keyName))
           throw std::runtime_error("Attempted to access Named key: "+ keyName+" but this key is not in the map, Typo?");
       return isPressed(m_keyMap.at(keyName));
    }

    void InputManager::mapKey(std::string keyName, wlo::Key::Code code) {
        m_keyMap[keyName] = code;
    }

    void InputManager::setKeyMap(std::initializer_list<std::pair<std::string, wlo::Key::Code>> mappings) {
            for (const auto &[keyName,code] : mappings)
                mapKey(keyName,code);
    }

    void InputManager::connect(Messenger *messenger) {
       auto pwind = dynamic_cast<Window *>(messenger);
       if(pwind!=nullptr){
           pwind->permit<KeyboardMessage,InputManager,&InputManager::recieveKey>(this);
       }
    }


}


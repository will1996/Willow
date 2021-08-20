//
// Created by Will Chambers on 3/2/21.
//

#ifndef WILLOW_INPUTMANAGER_HPP
#define WILLOW_INPUTMANAGER_HPP
#include<willow/messaging/MessageSystem.hpp>
#include<willow/window/Window.hpp>
#include<unordered_map>
namespace wlo {

    class InputManager :public EngineComponent{
    public:

        InputManager();
        void connect(Messenger * messenger)override;
        void setKeyMap(std::initializer_list<std::pair<std::string, wlo::Key::Code> > mappings);
        void mapKey(std::string,wlo::Key::Code);
        bool isPressed(wlo::Key::Code) const ;
        bool isPressed(std::string keyName)const ;
    private:
        void recieveKey(const KeyboardMessage& );
        std::unordered_map<std::string, wlo::Key::Code> m_keyMap;
        std::unordered_map<wlo::Key::Code,bool> m_pressedMap;
        void resetPressedMap();
    };

}
#endif //WILLOW_INPUTMANAGER_HPP

#ifndef WILLOW_MESSAGES_H
#define WILLOW_MESSAGES_H
#include<stdint.h>

#include "willow/root/Root.hpp"
#include "willow/input/KeyCodes.hpp"
#include "willow/input/MouseCodes.hpp"
namespace wlo{
//Messages are lean and mean structs, all operations on them are performed by static functions within the body of the extending class
//the raw template can be used anywhere for custom implementations, the messaging library can send them.
struct Message
{ };

enum class KeyAction{
    Pressed,Held,Released
};
//Keyboard Messages
 struct KeyInfo{
     Key::Code button;
     uint16_t mod_bundle = 0 ;
     uint32_t repeat_length = 0;
     KeyAction action;
 };
 struct KeyboardMessage : public Message{
    KeyInfo content;
     //True if the keyboard message has the specified modifier, useful for shift, control etc.
     static bool hasModifier(const KeyboardMessage & keyMessage, const Key::Modifier& mod) {
         return Key::hasModifer(keyMessage.content.mod_bundle,mod);
     }
 };
 struct KeyPressed : KeyboardMessage{
     KeyPressed(Key::Code button, Key::Modifier mod):
     KeyboardMessage{.content{button, mod, 0, KeyAction::Pressed}}{}
 };
 struct KeyReleased : KeyboardMessage{
     KeyReleased(Key::Code button, Key::Modifier mod):
     KeyboardMessage{.content{button, mod, 0, KeyAction::Released}}{}
 };
 struct KeyHeld : KeyboardMessage{
     KeyHeld(Key::Code button, Key::Modifier mod):
     KeyboardMessage{.content{button, mod, 1, KeyAction::Held}} { }
 };

 enum class WindowAction{
     Closed,Resized,GainedFocus,LostFocus
 };
//Window Messages
    struct WindowInfo  {
        std::string title;
        uint32_t width;
        uint32_t height;
        const WindowAction action;
    };

struct WindowClosed : public Message{
};
struct WindowResized : public Message{
    WindowInfo content;
    WindowResized(std::string title, uint32_t width, uint32_t height):
   content {title, width, height, WindowAction::Resized }{}
};
struct WindowGainedFocus: public Message{
};
struct WindowLostFocus: public Message{
};

struct CursorState{
    bool locked;
};
struct SetWindowCursorState:public Message{
    CursorState content;
};

//MouseMessages
struct MousePositionInfo{
    double xPos;
    double yPos;
};

enum class MouseClickAction{
    Pressed,Released,Held
};

struct MouseClickInfo :MousePositionInfo{
    Mouse::Code button;
    uint16_t mod_bundle;
    MouseClickAction action;
};

struct MouseScrollInfo {
    double xScroll_Offset;
    double yScroll_Offset;
};

struct MouseButtonMessage : Message{
    MouseClickInfo content;
};
struct MouseButtonPressed   : MouseButtonMessage{
    MouseButtonPressed(MousePositionInfo pos,Mouse::Code button,Key::Modifier mod ) :
    MouseButtonMessage{.content = {pos,button,mod,MouseClickAction::Pressed}}{}
};
struct MouseButtonReleased  : MouseButtonMessage{
    MouseButtonReleased(MousePositionInfo pos,Mouse::Code button,Key::Modifier mod) :
    MouseButtonMessage{.content = {pos,button,mod,MouseClickAction::Released}}{}
    };
struct MouseButtonHeld      : MouseButtonMessage{
    MouseButtonHeld(MousePositionInfo pos,Mouse::Code button,Key::Modifier mod ) :
    MouseButtonMessage{.content = {pos,button,mod,MouseClickAction::Held}}{}
};

struct MouseScrolled :Message{
    MouseScrollInfo content;

};

struct MouseMoved : Message{
    MousePositionInfo content;
};
// control flow messages
struct EngineStart:Message{};
struct FrameStart :Message{};
struct ReadInput :Message{};
struct RenderDataReady:Message{};
struct RenderStart :Message{};
struct EngineStop:Message{};
struct Shutdown:Message{};





}


inline std::ostream & operator<<(std::ostream& os, wlo::KeyInfo & data ){
    os<<"[Button: "<< wlo::Key::toText(data.button);
    if(data.repeat_length>0)
        os<<", Repeat("<<data.repeat_length<<")";
    os<<"]";
    return os;
}

inline std::ostream & operator<<(std::ostream& os, wlo::KeyboardMessage & msg ){
        os<<"Keyboard message: "<<msg.content;
        return os;
}

inline std::ostream & operator<<(std::ostream& os, const wlo::WindowInfo & data ){
    os<<"Window: "<<data.title<<" Width:"<<data.width<<" Height:"<<data.height;
    return os;
}


inline std::ostream & operator<<(std::ostream& os, const wlo::WindowClosed & msg ){
    os<<"Window Closed";
    return os;
}

inline std::ostream & operator<<(std::ostream& os, const wlo::WindowGainedFocus & msg ){
    os<<"Window Gained Focus ->  ";
    return os;
}

inline std::ostream & operator<<(std::ostream& os, const wlo::WindowLostFocus& msg ){
    os<<"Window Lost Focus ->  ";
    return os;
}

inline std::ostream & operator<<(std::ostream& os, const wlo::WindowResized& msg ){
    os<<"Window Reized to ->  "<<msg.content;
    return os;
}
inline std::ostream & operator<<(std::ostream& os, const wlo::MousePositionInfo & info ){
    os<<"Mouse Position ( "<<info.xPos<<","<<info.yPos<<")";
    return os;
}
inline std::ostream & operator<<(std::ostream& os, const wlo::MouseScrollInfo & info ){
    os<<"Mouse scrolled:";
    os<<"xOffset: "<<info.xScroll_Offset;
    os<<"yOffset: "<<info.yScroll_Offset;
    return os;
}

inline std::ostream & operator<<(std::ostream& os, const wlo::MouseClickInfo & info ){
    os<<"Mouse "<<wlo::Mouse::ButtonName(info.button)<<"Clicked: at ";
    os<<wlo::MousePositionInfo(info);
    if(info.mod_bundle)
        os<<wlo::Key::getBundleName(info.mod_bundle);
    return os;
}






#endif

#pragma once
#include<stdint.h>

#include "willow/root/wilo_dev_core.hpp"
namespace wlo{

    //TODO replace with templated class for static inheritance, forces static type resolution


/*Wilo uses signals as inter-component communication */


enum class MessageType{
    None = 0,
    MouseMoved,MouseButtonPressed,MouseButtonReleased, MouseScrolled,
    WindowResized, WindowGainFocus,WindowLoseFocus, WindowClose,
    KeyPressed,KeyHeld,KeyReleased,
    FrameStart,FrameEnd
};

static std::map<MessageType, std::string> NameOfMessageType{
    {MessageType::MouseMoved, "MouseMoved"},
    {MessageType::MouseButtonPressed, "MouseButtonPressed"},
    {MessageType::MouseButtonReleased, "MouseButtonReleased"},
    {MessageType::MouseScrolled, "MouseScrolled"},
    {MessageType::WindowResized, "WindowResized"},
    {MessageType::WindowGainFocus, "WindowGainFocus"},
    {MessageType::WindowLoseFocus, "WindowLoseFocus"},
    {MessageType::WindowClose, "WindowClose"},
    {MessageType::KeyPressed, "KeyPressed"},
    {MessageType::KeyHeld, "KeyHeld"},
    {MessageType::KeyReleased, "KeyReleased"},
    {MessageType::FrameStart, "FrameEnd"},
    {MessageType::FrameEnd, "FrameEnd"},
};

class Message
{
public:
    Message(){};
    virtual ~Message(){};
    virtual MessageType getType() const = 0;
    virtual std::string toString()   const = 0; 

    void mark_handled(){
        m_handled = true;
    }
    bool is_handled(){
        return m_handled;
    }
private:
    bool m_handled = false;
 };

    inline std::ostream& operator<<(std::ostream& o,const Message& s){
        return o<<s.toString(); 
    }


    //class EventDispatcher{
    //    public:
    //        EventDispatcher(Signal& e)
    //         :m_event(e)//initializer list type shit
    //         { }
    //            //allows for very cute notation, might replace with a switch statement
    //         template<typename T, typename F>
    //            bool dispatch(const F& func){
    //                    if(m_event.getEventType()==T::getStaticType()){
    //                        func(static_cast<T&>( m_event));
    //                        
    //                        return m_event.is_handled();
    //                    }
    //                return false;
    //            }
    //    private:
    //        Signal& m_event;


    //};
}

inline std::ostream& operator<<(std::ostream& os, wlo::MessageType s){
    os << wlo::NameOfMessageType[s];
    return os;
}

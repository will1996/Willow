#include "willow/wilo_console.hpp"
#include<thread>
#include<future>
#include<chrono>
namespace wlo{
    Console::Console(wlo::SharedPointer<wlo::lua::Environment> env):Scriptable<Console>("console",this,env),m_testScriptsPath(WILO_TESTS_SCRIPTS_PATH),m_engineScriptsPath(WILO_ENGINE_SCRIPTS_PATH){
        this->LT_appendMember<&Console::quit>("quit");
        this->LT_appendMember<&Console::resize>("resize");
        this->LT_appendMember<&Console::setMouse>("setMouse");
        this->LT_appendMember<&Console::pressKey>("pressKey");
        this->LT_appendMember<&Console::reinitialize>("reinitialize");
        m_env->setglobal("tests_path",m_testScriptsPath);
        m_env->runScript(m_testScriptsPath + "testing_base.lua");
    }
    Console::~Console(){
        //do nothing
    }
    void Console::initialize(){
        m_env->runScript(m_engineScriptsPath + "console_base.lua");
        WILO_CORE_INFO("console initialized!")

    }
   
    void Console::evaluate( std::string command ){
        m_env->runScript(command);
    }

    using namespace std::literals::chrono_literals;//give me the use of "s" for seconds
    std::string Console::readInput(){
            std::string command;
            std::cin >> command;
            return command;
    }

    void Console::run() {
    }

    void Console::setScriptPath(std::string path){
        m_userScriptsPath = path;
    }

    void Console::reclaim(){
        WILO_CORE_INFO("console reclamation started");

        WILO_CORE_INFO("console shutdown properly");
    }

    Subject* Console::asSubject()
    {
        return &this->subject;
    }

    void Console::checkIn(){
        std::chrono::seconds timeout(3);
        std::future<std::string> response = std::async(readInput);
        if(response.wait_for(timeout)==std::future_status::ready)
            evaluate(response.get());
    }

    void Console::notifyWindowObservers(const wlo::WindowMessage& msg){
        subject.notify<WindowMessage>(msg);
    }

    void Console::notifyMouseObservers(const wlo::MouseMessage& msg)
    {
        subject.notify<MouseMessage>(msg);
    }

    void Console::notifyKeyboardObservers(const wlo::KeyboardMessage& msg)
    {
        subject.notify<KeyboardMessage>(msg);
    }

    int Console::quit(::lua_State* L ){
        WindowMessage::Info info;
        info.title = "app window";
        notifyWindowObservers(wlo::WindowMessage(MessageType::WindowClose, info));
        return 0;
    }


    int Console::resize(::lua_State* L){
        int width = ::lua_tointeger(L, 1);
        int height = ::lua_tointeger(L, 2);

        WindowMessage::Info info;
        info.title = "app window";
        info.height = height;
        info.width = width;

        notifyWindowObservers(wlo::WindowMessage(MessageType::WindowResized, info));
        return 0;
    }

    int Console::setMouse(::lua_State* L)
    {
        int x = ::lua_tointeger(L, 1);
        int y = ::lua_tointeger(L, 2);
        MouseMessage::Info info;
        info.xPos = x;
        info.yPos = x;
        notifyMouseObservers(wlo::MouseMessage(MessageType::MouseMoved, info));
        return 0;
    }

    int Console::pressKey(::lua_State* L)
    {
        int code = ::lua_tointeger(L, -1);
        KeyboardMessage::Info info;
        info.button = wlo::KeyCode(code);
        notifyKeyboardObservers(wlo::KeyboardMessage(MessageType::KeyPressed, info));
        return 0;
    }
    int Console::reinitialize(::lua_State* L){
        m_env->runScript(m_engineScriptsPath + "console_base.lua");
        m_env->runScript(m_testScriptsPath + "testing_base.lua");
#ifndef ndebug
        Scriptable::L_reinitialize();
#endif
        return 0;
    }
}
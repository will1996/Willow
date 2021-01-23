#include"willow/root/wilo_dev_core.hpp"
#include "willow/messaging/MessageSystem.hpp"
#include"willow/scripting/LuaBinding.hpp"
#include "willow/rendering/Renderer.hpp"
#include"ConsoleCore.hpp"
#include<atomic>




namespace wlo{
class Console : public MessageSystem::Observer {
    public:
        Console(wlo::SharedPointer<wlo::lua::Environment>);
        void initialize() ;
        void evaluate(std::string command);
        void run();
        void render();
        void checkIn();
        virtual void reclaim();
        MessageSystem::Subject* asSubject();
        ~Console();


        void setScriptPath(std::string path);


        private:
        const static size_t startingWidth = 400;
        const static size_t startingHeight = 900;
    wlo::SharedPointer<Window> window;
        wlo::UniquePointer<rendering::Renderer> m_console_renderer;
        const std::string m_testScriptsPath;
        const std::string m_engineScriptsPath;
        std::string m_userScriptsPath;
        MessageSystem::Subject subject;
        LuaBinding<Console> scriptable;
        ConsoleCore core;
        void recieve(const wlo::KeyboardMessage& msg ) ;

//        void notifyWindowObservers  (const wlo::WindowMessage& msg);
        void notifyMouseObservers   (const wlo::MouseMoved& msg);
        void notifyKeyboardObservers(const wlo::KeyboardMessage& msg);
 //       void recieve(const wlo::WindowMessage& msg);
        void recieve(const wlo::MouseMoved& msg);
        void recieve(const wlo::MouseButtonMessage& msg);
        static std::string readInput();
        int quit(::lua_State *L);
        int resize(::lua_State* L);
        int setMouse(::lua_State* L);
        int pressKey(::lua_State* L);
        int reinitialize(::lua_State* L);
    };

}
#include"willow/root/wilo_dev_core.hpp"
#include"willow/messaging/wilo_subject.hpp"
#include"willow/messaging/window_message.hpp"
#include"willow/messaging/mouse_message.hpp"
#include"willow/messaging/keyboard_message.hpp"
#include"willow/scripting/LuaBinding.hpp"
#include "willow/rendering/wilo_renderer.hpp"
#include"ConsoleCore.hpp"
#include<atomic>




namespace wlo{
class Console : public Observer {
    public:
        Console(wlo::SharedPointer<wlo::lua::Environment>);
        void initialize() override;
        void evaluate(std::string command);
        void run();
        void render();
        void checkIn();
        virtual void reclaim() override;
        Subject* asSubject();
        ~Console();


        void setScriptPath(std::string path);


        private:
        const static size_t startingWidth = 400;
        const static size_t startingHeight = 900;
    wlo::SharedPointer<Window> window;
        wlo::UniquePointer<Renderer> m_console_renderer;
        const std::string m_testScriptsPath;
        const std::string m_engineScriptsPath;
        std::string m_userScriptsPath;
        Subject subject;
        LuaBinding<Console> scriptable;
        ConsoleCore core;
        void recieve(const wlo::KeyboardMessage& msg ) ;

        void notifyWindowObservers  (const wlo::WindowMessage& msg);
        void notifyMouseObservers   (const wlo::MouseMessage& msg);
        void notifyKeyboardObservers(const wlo::KeyboardMessage& msg);
        void recieve(const wlo::WindowMessage& msg);
        void recieve(const wlo::MouseMessage& msg);
        static std::string readInput();
        int quit(::lua_State *L);
        int resize(::lua_State* L);
        int setMouse(::lua_State* L);
        int pressKey(::lua_State* L);
        int reinitialize(::lua_State* L);
    };

}
#include"willow/root/wilo_dev_core.hpp"
#include"willow/messaging/wilo_subject.hpp"
#include"willow/messaging/window_message.hpp"
#include"willow/messaging/mouse_message.hpp"
#include"willow/messaging/keyboard_message.hpp"
#include"willow/scripting/Scriptable.hpp"
#include<atomic>




namespace wlo{
    class Console : public Scriptable<Console> {
    public:
        Console(wlo::SharedPointer<wlo::lua::Environment>);
        void initialize() override;
        void evaluate(std::string command);
        void run();
        void checkIn();
        void reclaim();
        Subject* asSubject();
        ~Console();

    
        void setScriptPath(std::string path);


        private:
        const std::string m_testScriptsPath;
        const std::string m_engineScriptsPath;
        std::string m_userScriptsPath;
        Subject subject;
        void notifyWindowObservers  (const wlo::WindowMessage& msg);
        void notifyMouseObservers   (const wlo::MouseMessage& msg);
        void notifyKeyboardObservers(const wlo::KeyboardMessage& msg);
        static std::string readInput();
        int quit(::lua_State *L);
        int resize(::lua_State* L);
        int setMouse(::lua_State* L);
        int pressKey(::lua_State* L);
        int reinitialize(::lua_State* L);
    };

}
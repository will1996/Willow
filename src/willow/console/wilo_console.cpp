#include "willow/console/wilo_console.hpp"
#include"glm/gtc/matrix_transform.hpp"
#include"glm/gtx/string_cast.hpp"
#include<thread>
#include<future>
#include<chrono>
#include"willow/rendering/camera/OrthographicCamera3D.hpp"
namespace wlo{
    Console::Console(wlo::SharedPointer<wlo::lua::Environment> env):scriptable("console",this,env),m_testScriptsPath(WILO_TESTS_SCRIPTS_PATH),m_engineScriptsPath(WILO_ENGINE_SCRIPTS_PATH){
        scriptable.Register<&Console::quit>("quit");
        scriptable.Register<&Console::resize>("resize");
        scriptable.Register<&Console::setMouse>("setMouse");
        scriptable.Register<&Console::pressKey>("pressKey");
        scriptable.Register<&Console::reinitialize>("reinitialize");
        env->setglobal("tests_path",m_testScriptsPath);
        env->runScript(m_testScriptsPath + "testing_base.lua");
        env->runScript(m_engineScriptsPath + "console_base.lua");
    }
    void Console::initialize(){
        wlo::Window::Info windowInfo;
        windowInfo.m_width = startingWidth;
        windowInfo.m_height = startingHeight;
        windowInfo.m_title = "Willow Script Console";
        window = wlo::SharedPointer<Window>(wilo_get_window(windowInfo));
        window->initialize();
        window->permit<KeyboardMessage, Console,&Console::recieve>(this) ;//register as an observer with the window, so we recieve events;

        wlo::Renderer::Info rendererInfo;
        rendererInfo.enableRendererStatistics = false;
        rendererInfo.enableGraphicsDebugging = true;
        rendererInfo.vertexBufferStartingSize = 100000;
        rendererInfo.indexBufferStartingSize = 120000;
        m_console_renderer =  wlo::CreateUniquePointer<Renderer>(window, rendererInfo);
        m_console_renderer->initialize();
        OrthographicCamera3D::BoundingBox viewingVolume;
        viewingVolume.left = 0;
        viewingVolume.right = windowInfo.m_width;
        viewingVolume.top = 0;
        viewingVolume.bottom = windowInfo.m_height;
        //glm::mat4x4 proj = OrthographicCamera3D(viewingVolume).getProj();
        float scale = 10;
        glm::mat4x4 proj =glm::scale(glm::mat4x4(1),glm::vec3(2/windowInfo.m_width,2/windowInfo.m_height,1));
        proj *= glm::translate(glm::mat4x4(1),glm::vec3(-float(windowInfo.m_width)/2,-float(windowInfo.m_height)/2,0));
        m_console_renderer->setCamera(glm::mat4x4(1),proj);
        m_console_renderer->setClearColor({0,0,0,1});
        window->permit<WindowMessage,Renderer,&Renderer::handleWindowResize>(m_console_renderer.get()) ;//register as an observer with the window, so we recieve events;
        window->permit<WindowMessage,Console, &Console::recieve>(this) ;//register as an observer with the window, so we recieve events;
        WILO_CORE_INFO("console initialized!")
    }

    void Console::render() {
        m_console_renderer->setClearColor({0,0,0,1});
        m_console_renderer->beginDrawCall();
        m_console_renderer->pushGeometry(core.getVertexBuffer(),core.getIndexBuffer(),glm::mat4x4(1));
        m_console_renderer->submitDrawCall();
        m_console_renderer->clearGeometryBuffers();
    }

    void Console::recieve(const wlo::WindowMessage& msg){
         auto info =  msg.getInfo();
        glm::mat4x4 proj =glm::scale(glm::mat4x4(1),glm::vec3(2/info.width,2/info.height,1));
        proj *= glm::translate(glm::mat4x4(1),glm::vec3(-float(info.width)/2,-float(info.height)/2,0));
        m_console_renderer->setProjection(proj);
    }
    void Console::evaluate( std::string command ){
        scriptable.getEnv()->runScript(command);
    }

    void Console::recieve(const KeyboardMessage &msg) {
        if (msg.getType() == wlo::MessageType::KeyPressed || msg.getType() == wlo::MessageType::KeyHeld) {
            if (msg.getInfo().button == wlo::KeyCode::DOWN) {
               // core.moveCursor(ConsoleCore::CursorMovement::DOWN);
            } else if (msg.getInfo().button == wlo::KeyCode::UP) {
                //core.moveCursor(ConsoleCore::CursorMovement::UP);
            } else if (msg.getInfo().button == wlo::KeyCode::LEFT) {
                core.moveCursor(ConsoleCore::CursorMovement::LEFT);
            } else if (msg.getInfo().button == wlo::KeyCode::RIGHT) {
                core.moveCursor(ConsoleCore::CursorMovement::RIGHT);
            } else if (msg.getInfo().button == KeyCode::BACKSPACE) {
                core.deleteChar();
            } else if (msg.getInfo().button == KeyCode::ENTER) {
                evaluate(core.flushInputBuffer());
                core.nextLine();
            } else if (msg.getInfo().button == KeyCode::ESCAPE) {
               notifyWindowObservers(wlo::WindowMessage(wlo::MessageType::WindowClose,{"Console"})) ;

            } else
                core.placeChar(msg.getInfo().button);
        }
    }


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
        scriptable.getEnv()->runScript(m_engineScriptsPath + "console_base.lua");
        scriptable.getEnv()->runScript(m_testScriptsPath + "testing_base.lua");
        return 0;
    }
    Console::~Console(){
        //do nothing
    }

    void Console::recieve(const MouseMessage &msg) {
        if(msg.getType()==MessageType::MouseScrolled){
           if(msg.getInfo().yScroll_Offset>0) {

           }
        }
    }

}
#include "willow/window/mac_window.hpp"
#include<vulkan/vulkan.h>
#include <GLFW/glfw3.h>

namespace wlo{


    class MacWindowImpl{
    public:
        MacWindowImpl(GLFWwindow* window): m_window(window){ }
        GLFWwindow* getWindow() { return m_window; };
        GLFWwindow* m_window = nullptr;
    };



    MacWindow::~MacWindow() {

    }
    
    void MacWindow::initialize(){
        bool result = glfwInit();
        WILO_ASSERT(result);
        WILO_CORE_INFO("Creating a window with title {0}, and dimensions ({1},{2})",m_info.m_title,m_info.m_height,m_info.m_width);

        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);//this is required for using Vulkan with this window
        p_impl = wlo::CreateUniquePointer<MacWindowImpl>(glfwCreateWindow(m_info.m_width
                        , m_info.m_height
                        , m_info.m_title.c_str()
                        , nullptr//we don't specify a monitor (yet)
                        , nullptr//we don't want to use share funcitonality
                        ));
        WILO_ASSERT(p_impl->getWindow()!=nullptr);
            //give GLFW a reference to this macWindow instance such that our
                //glfw callbacks can access "this"
        glfwSetWindowUserPointer(p_impl->getWindow(),this);

        glfwSetErrorCallback([](int error,const char* error_description){
            WILO_CORE_ERROR(error_description);
        });

        glfwSetKeyCallback(p_impl->getWindow(),[](GLFWwindow* window, int key, int scancode, int action,int mods){
            MacWindow* instance  =  (MacWindow*) (glfwGetWindowUserPointer(window));
            Key::Code button = Key::Code(key);
            Key::Modifier mod_bundle = Key::Modifier(mods);
            if(action==GLFW_PRESS){
                instance -> notifyKeyObservers(wlo::KeyPressed{button,mod_bundle});
            }else if(action ==GLFW_REPEAT){
                instance -> notifyKeyObservers(wlo::KeyHeld{button,mod_bundle});
            }else{//action == GLFW_RELEASE
                instance -> notifyKeyObservers(wlo::KeyReleased{button,mod_bundle});
            } 
        });

        glfwSetMouseButtonCallback(p_impl->getWindow(),[](GLFWwindow* window, int button, int action,int mods){
            MacWindow* instance = (MacWindow*) (glfwGetWindowUserPointer(window));
            Mouse::Code buttoncode = Mouse::Code(button);
            Key::Modifier mod_bundle = Key::Modifier(mods);
            double xpos,ypos;
            glfwGetCursorPos(window,&xpos,&ypos);

            if(action == GLFW_PRESS){
                instance -> notifyMouseObservers(MouseButtonPressed{MousePositionInfo{xpos,ypos},buttoncode,mod_bundle});
            }else if(action ==GLFW_REPEAT) {
                instance -> notifyMouseObservers(MouseButtonHeld{MousePositionInfo{xpos,ypos},buttoncode,mod_bundle});
            }else{//action ==GLFW_RELEASE
                instance -> notifyMouseObservers(MouseButtonReleased{MousePositionInfo{xpos,ypos},buttoncode,mod_bundle});
            }
        });
        glfwSetCursorPosCallback(p_impl->getWindow(),[](GLFWwindow* window, double xpos, double ypos){
            MacWindow* instance = (MacWindow*)(glfwGetWindowUserPointer(window));
            instance -> notifyMouseObservers(MouseMoved{xpos,ypos});
        });
        glfwSetScrollCallback(p_impl->getWindow(),[](GLFWwindow* window, double xpos, double ypos){
            MacWindow* instance = (MacWindow*)(glfwGetWindowUserPointer(window));
            instance ->notifyMouseObservers(MouseScrolled{xpos,ypos});
        });
    
        glfwSetWindowSizeCallback(p_impl->getWindow(),[](GLFWwindow* window, int width, int height){
            MacWindow* instance = (MacWindow*)(glfwGetWindowUserPointer(window));

            //resize the window
            instance->m_info.m_height = height;
            instance->m_info.m_width = width;

             std::string title = instance->getInfo().m_title;
            instance -> notifyWindowObservers( WindowResized{title,static_cast<uint32_t>(width),static_cast<uint32_t>(height)});
        });

        glfwSetWindowFocusCallback(p_impl->getWindow(),[](GLFWwindow* window, int Focused){

            MacWindow* instance = (MacWindow*)(glfwGetWindowUserPointer(window));
            if(Focused==GLFW_FOCUSED) {
                instance->notifyWindowObservers(WindowGainedFocus{instance->getInfo().m_title,instance->getInfo().m_width,instance->getInfo().m_height});
            }else{
                instance->notifyWindowObservers(WindowLostFocus{instance->getInfo().m_title,instance->getInfo().m_width,instance->getInfo().m_height});
            }

            });
        glfwSetWindowCloseCallback(p_impl->getWindow(),[](GLFWwindow* window ){

            MacWindow* instance = (MacWindow*)(glfwGetWindowUserPointer(window));
            instance->notifyWindowObservers(WindowClosed{instance->getInfo().m_title,instance->getInfo().m_width,instance->getInfo().m_height});
        });

        WILO_CORE_INFO("Window initialized!")
    }

    void MacWindow::notifyKeyObservers(const wlo::KeyboardMessage& msg){
            this->notify<KeyboardMessage>(msg);
    }
    void MacWindow::notifyMouseObservers(const wlo::MouseButtonMessage& msg){
            this->notify<MouseButtonMessage>(msg);
    }
    void MacWindow::notifyMouseObservers(const wlo::MouseMoved& msg){
        this->notify<MouseMoved>(msg);
    }

    void MacWindow::notifyMouseObservers(const wlo::MouseScrolled & msg){
        this->notify<MouseScrolled>(msg);
    }
    void MacWindow::notifyWindowObservers(const wlo::WindowClosed& msg){
            this->notify<WindowClosed>(msg);
    }

    void MacWindow::notifyWindowObservers(const wlo::WindowResized& msg){
        this->notify<WindowResized>(msg);
    }

    void MacWindow::notifyWindowObservers(const wlo::WindowLostFocus& msg){
        this->notify<WindowLostFocus>(msg);
    }

    void MacWindow::notifyWindowObservers(const wlo::WindowGainedFocus& msg){
        this->notify<WindowGainedFocus>(msg);
    }

    Window::Info MacWindow::getInfo() const{
        return m_info;
    }
    void MacWindow::checkIn(){
        glfwPollEvents(); //let glfw trigger all callbacks
    };
    void MacWindow::reclaim(){
        glfwTerminate();
    };

    bool MacWindow::shouldClose(bool query){
        if(query)
        checkIn();
        return glfwWindowShouldClose(p_impl->getWindow());
    }

    void* MacWindow::getNativeWindow() const
    {
        std::cout << "getting native Window!" << std::endl;
        return p_impl->getWindow();
    }

    MacWindow::MacWindow(Window::Info info) {
        m_info.m_extraData = info.m_extraData;
        m_info.m_width = info.m_width;
        m_info.m_height = info.m_height;
        m_info.m_title = info.m_title;
        m_info.API = info.API;
        initialize();

    }


   void MacWindow::setCursorMode(bool locked) {
        if(locked)
        glfwSetInputMode(p_impl->getWindow(),GLFW_CURSOR,GLFW_CURSOR_DISABLED);
        else
        glfwSetInputMode(p_impl->getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
   }

}

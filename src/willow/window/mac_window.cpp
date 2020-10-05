#include "willow/window/mac_window.hpp"
#include "willow/messaging/keyboard_message.hpp"
#include<vulkan/vulkan.h>
#include <GLFW/glfw3.h>

namespace wlo{


    class MacWindowImpl{
    public:
        MacWindowImpl(GLFWwindow* window): m_window(window){
             
        
        }
        GLFWwindow* getWindow() {
            return m_window;
        };


            GLFWwindow* m_window = nullptr;
    
    };


    MacWindow::MacWindow(wlo::Window::Info &info){
        m_info.m_extraData = info.m_extraData;
        m_info.m_width = info.m_width;
        m_info.m_height = info.m_height;
        m_info.m_title = info.m_title;
        m_info.API = info.API;
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
            KeyboardMessage::Info info; 
            info.button = KeyCode(key);
            info.mod_bundle = KeyModifier(mods);
            if(action==GLFW_PRESS){
                info.repeat_length = 0;
                instance -> notifyKeyObservers(wlo::KeyboardMessage(MessageType::KeyPressed, info));
            }else if(action ==GLFW_REPEAT){
                info.repeat_length = 1;
                instance -> notifyKeyObservers(wlo::KeyboardMessage(MessageType::KeyHeld, info));
            }else{//action == GLFW_RELEASE
                info.repeat_length = -1;
                instance -> notifyKeyObservers(wlo::KeyboardMessage(MessageType::KeyReleased, info));
            } 
        });

        glfwSetMouseButtonCallback(p_impl->getWindow(),[](GLFWwindow* window, int button, int action,int mods){
            MacWindow* instance = (MacWindow*) (glfwGetWindowUserPointer(window));
            MouseMessage::Info info;
            info.button = MouseButton(button);  
            info.mod_bundle = KeyModifier(mods);
            double xpos,ypos;
            glfwGetCursorPos(window,&xpos,&ypos);
            info.xPos = xpos;
            info.yPos = ypos;
            if(action == GLFW_PRESS){
                instance -> notifyMouseObservers(wlo::MouseMessage(MessageType::MouseButtonPressed, info));
            }else {//action ==GLFW_RELEASE
                instance -> notifyMouseObservers(wlo::MouseMessage(MessageType::MouseButtonReleased, info));
            }
        });
        glfwSetCursorPosCallback(p_impl->getWindow(),[](GLFWwindow* window, double xpos, double ypos){
            MacWindow* instance = (MacWindow*)(glfwGetWindowUserPointer(window));
            MouseMessage::Info info;
            info.xPos = xpos;
            info.yPos = ypos;
            instance ->notifyMouseObservers(wlo::MouseMessage(MessageType::MouseMoved, info));
        });
        glfwSetScrollCallback(p_impl->getWindow(),[](GLFWwindow* window, double xpos, double ypos){
            MacWindow* instance = (MacWindow*)(glfwGetWindowUserPointer(window));
            MouseMessage::Info info;
            info.xScroll_Offset = xpos;
            info.yScroll_Offset = ypos;
            instance ->notifyMouseObservers(wlo::MouseMessage(MessageType::MouseScrolled, info));
        });
    
        glfwSetWindowSizeCallback(p_impl->getWindow(),[](GLFWwindow* window, int width, int height){
            MacWindow* instance = (MacWindow*)(glfwGetWindowUserPointer(window));

            //resize the window
            instance->m_info.m_height = height;
            instance->m_info.m_width = width;

            WindowMessage::Info info;
            info.height = height;
            info.width = width;
            info.title = instance->getInfo().m_title;
            instance -> notifyWindowObservers( WindowMessage(MessageType::WindowResized,info));

        });

        glfwSetWindowFocusCallback(p_impl->getWindow(),[](GLFWwindow* window, int Focused){

            MacWindow* instance = (MacWindow*)(glfwGetWindowUserPointer(window));
            WindowMessage::Info info;
            info.title = instance->getInfo().m_title;
            if(Focused==GLFW_FOCUSED){
            instance -> notifyWindowObservers( WindowMessage(MessageType::WindowResized,info));
            }

        });

        glfwSetWindowCloseCallback(p_impl->getWindow(),[](GLFWwindow* window ){

            MacWindow* instance = (MacWindow*)(glfwGetWindowUserPointer(window));
            WindowMessage::Info info;
            info.title = instance->getInfo().m_title;
            instance -> notifyWindowObservers( WindowMessage(MessageType::WindowClose,info));


        });

        WILO_CORE_INFO("Window initialized!")
    }

    void MacWindow::notifyKeyObservers(const wlo::KeyboardMessage& msg){
            this->notify<KeyboardMessage>(msg);
    }
    void MacWindow::notifyMouseObservers(const wlo::MouseMessage& msg){
            this->notify<MouseMessage>(msg);
    }
    void MacWindow::notifyWindowObservers(const wlo::WindowMessage& msg){
            this->notify<WindowMessage>(msg);
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

    bool MacWindow::shouldClose(){
        return glfwWindowShouldClose(p_impl->getWindow());
    }

    void* MacWindow::getNativeWindow()
    {
        std::cout << "getting native Window!" << std::endl;
        return p_impl->getWindow();
    }


}

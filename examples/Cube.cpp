//
// Created by W on 9/20/20.
//


#include "willow.hpp"
#include<iostream>
#include"glm/gtc/matrix_transform.hpp"

class CubeExample : public wlo::Application{
public:
    CubeExample(std::string entryPoint):Application(entryPoint){
        Application::Info inf("cubeExample",0);
        Application::initialize(inf);
        m_main_window->permit<wlo::MouseButtonMessage,CubeExample,&CubeExample::handleMouse>(this);
    }
    void handleMouse(const wlo::MouseButtonMessage& msg){

    }
    void run() override{
        WILO_INFO("running!")
        while(!m_shutting_down){
            m_renderer->setClearColor({0,0,0,1});
            m_renderer->beginDrawCall();

            std::vector<wlo::Vertex3D> triangle{
                    {{-0.5f, -0.5f,0.0f},{1.0f, 0.0f, 0.0f}},
                    {{0.5f, -0.5f,0.0f},{0.0f, 1.0f, 0.0f}},
                    {{0.5f, 0.5f,0.0f},{0.0f, 0.0f, 1.0f}},
            };

            std::vector<uint32_t> indices = {0,1,2};

            glm::mat4x4 view = glm::mat4x4(1);
            glm::mat4x4 proj = glm::mat4x4(1);//glm::ortho(0.0f, 600.0f, 0.0f, 800.0f);
            glm::mat4x4 model = glm::rotate(glm::mat4x4 (1),glm::radians(90.0f),{0,0,1});
            m_renderer->setCamera(view, proj);
            m_renderer->pushGeometry(triangle,indices,model);
            m_renderer->submitDrawCall();
            m_main_window->checkIn();
            m_console->render();
        }
    }


   ~ CubeExample(){
        Application::reclaim();
    }

};


WILLOW_TRUNK(CubeExample);


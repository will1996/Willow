//
// Created by W on 9/20/20.
//


#include "willow.hpp"
#include<iostream>


class CubeExample : public wlo::Application{
public:
    void start(){
        Application::initialize();
    }
    void run(){
        Application::run();
        WILO_INFO("running!")
    }
    void stop(){
        Application::reclaim();
    }

};


int main(){
    CubeExample app;
    app.start();
    app.run();
    app.stop();
}



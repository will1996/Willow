//
// Created by W on 9/24/20.
//
#include "willow/messaging/Xylem.h"
#include<string>
#include<cassert>
#include<chrono>
#include<iostream>


struct StandardInfo {
    StandardInfo(std::string name):m_name(name){};
    std::string m_name;
};

class TwoWay1{
public:
    bool heardBack;
    struct Info{
        Info(std::string job):m_job(job){};
       std::string m_job;
    };
   TwoWay1() = default;
   void reachoutStandard();
   void reachoutCustom();
   void recieve(const Xylem::Message<StandardInfo> &);
};

class TwoWay2{
public:
    bool gotorders;
    TwoWay2() = default;
    void reachoutStandard();
    void recieve(const Xylem::Message<TwoWay1::Info> &);
    void recieve(const Xylem::Message<StandardInfo> &);
};

void TwoWay1::reachoutStandard() {
    Xylem::sendStaticDirect<TwoWay1, TwoWay2, StandardInfo>(StandardInfo("hi from TwoWay1"));
}

void TwoWay1::reachoutCustom() {
    Xylem::sendStaticDirect<TwoWay1, TwoWay2, TwoWay1::Info>(TwoWay1::Info("hi from TwoWay1 with spice"));
}


void TwoWay1::recieve(const Xylem::Message<StandardInfo> &) {
    heardBack = true;
}

void TwoWay2::recieve(const Xylem::Message<StandardInfo> &) {
    gotorders = true;
    reachoutStandard();
}

void TwoWay2::recieve(const Xylem::Message<TwoWay1::Info> &) {
    gotorders = true;
    reachoutStandard();
}

void TwoWay2::reachoutStandard() {
    Xylem::sendStaticDirect<TwoWay2, TwoWay1, StandardInfo>(StandardInfo("hi from TwoWay1"));
}



int main(){
    TwoWay1 tw1;
    TwoWay2 tw2;
    Xylem::buildStaticChannel<TwoWay1,TwoWay2,StandardInfo>(&tw2);
    Xylem::buildStaticChannel<TwoWay1,TwoWay2,TwoWay1::Info>(&tw2);
    Xylem::buildStaticChannel<TwoWay2,TwoWay1,StandardInfo>(&tw1);
    tw1.reachoutStandard();
    assert(tw1.heardBack);
    assert(tw2.gotorders);
    auto start = std::chrono::high_resolution_clock::now();
   for(size_t i = 0; i < 100000000; i++)
       tw1.reachoutStandard();
    auto end = std::chrono::high_resolution_clock::now();
    auto diff  = std::chrono::duration<double>(end-start);
    std::cout<<"time per static direct message = " <<diff.count()/100000000<<std::endl;
}
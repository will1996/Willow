//
// Created by Will Chambers on 2/18/21.
//

#include"willow/root/Tag.hpp"

struct Test : wlo::Tag{
    std::string name;
};

class TestClass : public wlo::Tag{
    std::string name;
};

using namespace std;
int main(){

    Test hannah{.name = "hannah"};
    Test julliet{.name = "julliet"};
    Test julliet2 = julliet;
    Test julliet3{julliet};
    if(hannah.id==julliet.id) {
        std::cout<<"ID's are not unique"<<std::endl;
        return -1;
    }
    if(julliet2.id!=julliet.id) {
        std::cout<<"ID's are not copied correctly on assignment"<<std::endl;
        return -1;
    }
    if(julliet3.id!=julliet.id) {
        std::cout<<"ID's are not copied correctly on construction"<<std::endl;
        return -1;
    }

    TestClass hannahClass;
    TestClass jullietClass;
    TestClass jullietClass2 = jullietClass;
    TestClass jullietClass3{jullietClass};

    if(hannahClass.id==jullietClass.id) {
        std::cout<<"ID's on classes are not unique"<<std::endl;
        return -1;
    }
    if(hannahClass.id==0) {
        std::cout<<"ID's on classes are not correctly constructed"<<std::endl;
        return -1;
    }

    if(jullietClass2.id!=jullietClass.id) {
        std::cout<<"ID's are not copied correctly on assignment"<<std::endl;
        return -1;
    }
    if(jullietClass3.id!=jullietClass.id) {
        std::cout<<"ID's are not copied correctly on construction"<<std::endl;
        return -1;
    }
    std::cout<<"TAG TEST SUCCESSFUL"<<std::endl;
}
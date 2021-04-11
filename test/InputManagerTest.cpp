//
// Created by Will Chambers on 3/4/21.
//
#include"willow/input/InputManager.hpp"
#include<iostream>
using namespace wlo;
using namespace std;

class WindowDummy: public Window{
};

int main(){
WindowDummy wind;
InputManager input(wind);
if(input.isPressed(wlo::Key::Code::SPACE)) {
    cout<<"keys are not false by default"<<endl;
    return -1;
}
input.mapKey("test",wlo::Key::Code::SPACE);
bool exceptionThrown = false;
try{
   input.isPressed("Test");
}catch (const runtime_error& e){
   exceptionThrown = true;
}
if(!exceptionThrown) {
    cout<<"InputManager does not prevent access of un-mapped keys"<<endl;
    return -1;
}
}
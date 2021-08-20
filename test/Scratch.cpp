//
// Created by Will Chambers on 8/14/21.
//
#include<typeinfo>
#include<iostream>
template<typename T>
    void perType(int I) {
    std::cout << "index " << I << "got type of" << typeid(T).name() << std::endl;
}


template<typename ...Ts>
    void groupOfTypes(int i){
        (perType<Ts>(i),...);
    }


    int main(){
        groupOfTypes<int,float,char>(7);
    }
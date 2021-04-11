//
// Created by Will Chambers on 3/22/21.
//

#ifndef WILLOW_TESTINGTOOLS_HPP
#define WILLOW_TESTINGTOOLS_HPP
#include<iostream>
#include<string>

#define require(condition) if(!(condition)) {std::cout<<"condition" <<std::string(#condition) <<"Failed! " <<std::endl; return -1 ;}

#endif //WILLOW_TESTINGTOOLS_HPP

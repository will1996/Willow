//
// Created by Will Chambers on 8/19/21.
//
#include "TestingTools.hpp"
#include"willow/data/Data.hpp"
using namespace wlo::data;
static std::vector<Type::Member> members = {{"0",&wlo::Data::type<float>()},
                                     {"1",&wlo::Data::type<float>()}
};
int main(){
    auto members2 = std::move(members);

}
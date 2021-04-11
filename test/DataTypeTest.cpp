//
// Created by Will Chambers on 3/28/21.
//
#include"willow/root/Root.hpp"
#include"willow/data/Type.hpp"
#include<iostream>
#include"TestingTools.hpp"


using namespace std;
using namespace wlo;
int main(){

    require(wlo::data::Type::of<int>().footprint()==sizeof(int));
    require(wlo::data::Type::of<int>()==wlo::data::Type::of<int>());

    require(wlo::data::Type::of<float>().footprint()==sizeof(float));
    require(wlo::data::Type::of<float>()==wlo::data::Type::of<float>());
    require(wlo::data::Type::of<float>()!=wlo::data::Type::of<int>());

    require(wlo::data::Type::of<double>().footprint()==sizeof(double));
    require(wlo::data::Type::of<double>()==wlo::data::Type::of<double>());
    require(wlo::data::Type::of<double>()!=wlo::data::Type::of<int>());
    require(wlo::data::Type::of<double>()!=wlo::data::Type::of<float>());

    require(wlo::data::Type::of<char>().footprint()==sizeof(char));
    require(wlo::data::Type::of<char>()==wlo::data::Type::of<char>());
    require(wlo::data::Type::of<char>()!=wlo::data::Type::of<int>());
    require(wlo::data::Type::of<char>()!=wlo::data::Type::of<float>());
    require(wlo::data::Type::of<char>()!=wlo::data::Type::of<double>());

    require(wlo::data::Type::of<bool>().footprint()==sizeof(bool));
    require(wlo::data::Type::of<bool>()==wlo::data::Type::of<bool>());
    require(wlo::data::Type::of<bool>()!=wlo::data::Type::of<int>());
    require(wlo::data::Type::of<bool>()!=wlo::data::Type::of<float>());
    require(wlo::data::Type::of<bool>()!=wlo::data::Type::of<double>());
    require(wlo::data::Type::of<bool>()!=wlo::data::Type::of<char>());

    require(wlo::data::Type::of<Vec2>().footprint()==sizeof(Vec2));
    require(wlo::data::Type::of<Vec3>().footprint()==sizeof(Vec3));
    require(wlo::data::Type::of<Vec4>().footprint()==sizeof(Vec4));
    require(wlo::data::Type::of<Mat3>().footprint()==sizeof(Mat3));
    require(wlo::data::Type::of<Mat4>().footprint()==sizeof(Mat4));


    int counter1 = 0;
    auto members = wlo::data::Type::of<wlo::Vec3>().getMembers();
    for(const wlo::data::Type::Member  & member : members)
        counter1++;
    require(counter1==3);

    int counter2 = 0;
    for(size_t i=0; i < wlo::data::Type::of<wlo::Vec3>().getMembers().size();i++)
        counter2++;
    require(counter2==3);

    int counter3 = 0;
    for(auto member : wlo::data::Type::of<wlo::Vec3>().getMembers())
        counter3++;
    require(counter3==3);

}
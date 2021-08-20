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

   require(data::typeOf<int>().footprint()==sizeof(int));
    require(data::typeOf<int>().isPrimitive());
   require(data::typeOf<int>()==data::typeOf<int>());
   require(data::typeOf<float>().footprint()==sizeof(float));
   require(data::typeOf<float>()==data::typeOf<float>());
    require(data::typeOf<float>().isPrimitive());
   require(data::typeOf<float>()!=data::typeOf<int>());

   require(data::typeOf<double>().footprint()==sizeof(double));
    require(data::typeOf<double>().isPrimitive());
   require(data::typeOf<double>()==data::typeOf<double>());
   require(data::typeOf<double>()!=data::typeOf<int>());
   require(data::typeOf<double>()!=data::typeOf<float>());

   require(data::typeOf<char>().footprint()==sizeof(char));
    require(data::typeOf<char>().isPrimitive());
   require(data::typeOf<char>()==data::typeOf<char>());
   require(data::typeOf<char>()!=data::typeOf<int>());
   require(data::typeOf<char>()!=data::typeOf<float>());
   require(data::typeOf<char>()!=data::typeOf<double>());

   require(data::typeOf<bool>().footprint()==sizeof(bool));
   require(data::typeOf<bool>()==data::typeOf<bool>());
   require(data::typeOf<bool>()!=data::typeOf<int>());
   require(data::typeOf<bool>()!=data::typeOf<float>());
   require(data::typeOf<bool>()!=data::typeOf<double>());
   require(data::typeOf<bool>()!=data::typeOf<char>());
    require(data::typeOf<bool>().isPrimitive());


   require(data::typeOf<Vec2>().footprint()==sizeof(Vec2));
   require(data::typeOf<Vec3>().footprint()==sizeof(Vec3));


   require(data::typeOf<Vec4>().footprint()==sizeof(Vec4));
   require(data::typeOf<Mat3>().footprint()==sizeof(Mat3));
   require(data::typeOf<Mat4>().footprint()==sizeof(Mat4));

    auto t = data::typeOf<wlo::Vec3>();

   int counter1 = 0;
   auto members = data::typeOf<wlo::Vec3>().getMembers();
   for(const wlo::data::Type::Member  & member : members)
       counter1++;
   require(counter1==3);

   int counter2 = 0;
   for(size_t i=0; i < data::typeOf<wlo::Vec3>().getMembers().size();i++)
       counter2++;
   require(counter2==3);

   int counter3 = 0;
   for(auto member : data::typeOf<wlo::Vec3>().getMembers())
       counter3++;
   require(counter3==3);

}
//
// Created by Will Chambers on 3/28/21.
//
#include"willow/root/Root.hpp"
#include"willow/data/Data.hpp"
#include<iostream>
#include"TestingTools.hpp"


using namespace std;
using namespace wlo;
int main(){

   require(wlo::Data::type<int>().footprint()==sizeof(int));
   require(wlo::Data::type<int>()==wlo::Data::type<int>());
   require(wlo::Data::type<float>().footprint()==sizeof(float));
   require(wlo::Data::type<float>()==wlo::Data::type<float>());
   require(wlo::Data::type<float>()!=wlo::Data::type<int>());

   require(wlo::Data::type<double>().footprint()==sizeof(double));
   require(wlo::Data::type<double>()==wlo::Data::type<double>());
   require(wlo::Data::type<double>()!=wlo::Data::type<int>());
   require(wlo::Data::type<double>()!=wlo::Data::type<float>());

   require(wlo::Data::type<char>().footprint()==sizeof(char));
   require(wlo::Data::type<char>()==wlo::Data::type<char>());
   require(wlo::Data::type<char>()!=wlo::Data::type<int>());
   require(wlo::Data::type<char>()!=wlo::Data::type<float>());
   require(wlo::Data::type<char>()!=wlo::Data::type<double>());

   require(wlo::Data::type<bool>().footprint()==sizeof(bool));
   require(wlo::Data::type<bool>()==wlo::Data::type<bool>());
   require(wlo::Data::type<bool>()!=wlo::Data::type<int>());
   require(wlo::Data::type<bool>()!=wlo::Data::type<float>());
   require(wlo::Data::type<bool>()!=wlo::Data::type<double>());
   require(wlo::Data::type<bool>()!=wlo::Data::type<char>());



 require(wlo::Data::type<Vec2>().footprint()==sizeof(Vec2));
 require(wlo::Data::type<Vec3>().footprint()==sizeof(Vec3));
   require(wlo::Data::type<Vec4>().footprint()==sizeof(Vec4));
   require(wlo::Data::type<Mat3>().footprint()==sizeof(Mat3));
   require(wlo::Data::type<Mat4>().footprint()==sizeof(Mat4));


   int counter1 = 0;
   auto members = wlo::Data::type<wlo::Vec3>().getMembers();
   for(const wlo::data::Type::Member  & member : members)
       counter1++;
   require(counter1==3);

   int counter2 = 0;
   for(size_t i=0; i < wlo::Data::type<wlo::Vec3>().getMembers().size();i++)
       counter2++;
   require(counter2==3);

   int counter3 = 0;
   for(auto member : wlo::Data::type<wlo::Vec3>().getMembers())
       counter3++;
   require(counter3==3);

}
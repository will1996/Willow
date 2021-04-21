//
// Created by Will Chambers on 3/26/21.
//

#include "willow/data/Value.hpp"
#include "willow/root/Root.hpp"
#include"TestingTools.hpp"
#include<string>
using namespace std;


wlo::data::Value buildValue(std::string data){
    return wlo::data::Value(std::move(data));
}

int main(){

    int Int= 40;
    wlo::data::Value genericInt = Int;
    auto backInt = genericInt.get<int>();
    require(backInt==Int);
    genericInt = 35;
    require(genericInt.get<int>()==35);


    wlo::Vec3 vec{1,2,3};
    wlo::data::Value genericVec =  wlo::data::Value(vec);
    auto back = genericVec.get<wlo::Vec3>();
    require(vec==back);
    require(genericVec["x"].get<float>()==1.0f);
    require(genericVec["y"].get<float>()==2.0f);
    require(genericVec["z"].get<float>()==3.0f);

    genericVec["x"] = 40.0f;
    require(genericVec["x"].get<float>()==40.0f);
    wlo::Vec3 genericVec2 = wlo::Vec3{40.0f,2.0f,3.0f};
    require(genericVec.get<wlo::Vec3>()==genericVec2);


    wlo::Mat4 mat = glm::rotate(wlo::Mat4{0},90.0f,wlo::Vec3{0,1,0});
    wlo::data::Value genericMat =  mat;
   auto backMat = genericMat.get<wlo::Mat4>();
    require(mat==backMat);


   std::string testString( "this would be nice to pass" ) ;
   wlo::data::Value genericString(testString);
   auto backString = genericString.get<std::string>();
   wlo::data::Value doubleWrappedString = wlo::data::Value(genericString);



   require(backString==testString);



   wlo::data::Value constructedVec(wlo::data::Type::of<wlo::Vec3>());
   constructedVec["x"] = 30.5f;
   constructedVec["y"] = 40.5f;
   constructedVec["z"] = 45.5f;
   require(constructedVec["x"].get<float>()==30.5f) ;
   require(constructedVec["y"].get<float>()==40.5f) ;
   require(constructedVec["z"].get<float>()==45.5f) ;


    auto constructedBack = constructedVec.get<wlo::Vec3>();
    wlo::Vec3 target{30.5f,40.5f,45.5f};
    require(constructedBack==target);


   //test copy semantics


    wlo::data::Value copyValue = constructedVec;
    //copies are equal
    require(copyValue.get<wlo::Vec3>()==constructedVec.get<wlo::Vec3>());

    copyValue["x"]=4.5f;
    copyValue["y"]=3.5f;
    copyValue["z"]=2.5f;
    wlo::Vec3 copyTarget{4.5f,3.5f,2.5f};
    //copies are disjoint
    require(constructedVec.get<wlo::Vec3>()==target);
    require(copyValue.get<wlo::Vec3>()==copyTarget);


   //tests for nested structs
   wlo::data::Value genericMatrix = glm::rotate(wlo::Mat4{0},90.0f,wlo::Vec3{0,1,0});
   genericMatrix["r0"] = wlo::Vec4{0,1,2,3};
   genericMatrix["r1"] = wlo::Vec4{1,2,3,4};
   genericMatrix["r2"] = wlo::Vec4{2,3,4,5};
   genericMatrix["r3"] = wlo::Vec4{3,4,5,6};


   wlo::Mat4 truthMatrix{
       {0,1,2,3},
       {1,2,3,4},
       {2,3,4,5},
       {3,4,5,6}
   };

   require(truthMatrix==genericMatrix.get<wlo::Mat4>());

   wlo::data::Value copyMat = genericMatrix;
   //copies are equal
   require(copyMat.get<wlo::Mat4>()==genericMatrix.get<wlo::Mat4>());
   //copies are disjoint
   copyMat["r0"] = wlo::Vec4{32,45,44,57};

    wlo::Mat4 copyTruthMatrix{
            {32,45,44,57},
            {1,2,3,4},
            {2,3,4,5},
            {3,4,5,6}
    };
    require(genericMatrix.get<wlo::Mat4>()==truthMatrix);
    require(copyMat.get<wlo::Mat4>()==copyTruthMatrix);

    //tests for strings:
    std::string testString2 = "this is a test string";
    wlo::data::Value genericString2 = testString2;

    wlo::data::Value copyString = genericString2;
    require(copyString.get<string>()==genericString2.get<string>());
   // copyString = std::string("forth");
   // require(copyString.get<string>()=="forth");
   // require(genericString2.get<string>()==testString2)


   // auto moveToMe = buildValue("hehe").get<std::string>();

   // int counter=0;
   // for(auto member : constructedVec.getType().getMembers())
   //     counter++;
   // require(counter==3);

}
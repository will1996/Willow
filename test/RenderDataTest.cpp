//
// Created by W on 12/5/20.
//
#include"willow/data/Data.hpp"
#include<iostream>
using namespace wlo;
using namespace wlo::data;
using namespace std;
#include"TestingTools.hpp"
struct ShaderData{
    Vec3 position;
    Vec2 lightDirection;
};
struct Vertex3D{
    Vec3 position;
};

template<>
Type wlo::data::typeOf<ShaderData>(){
    return wlo::data::Type(
            "ShaderData",
            {
                    {"",data::typeOf<Vec3>()},
                    {"",data::typeOf<Vec2>()}
            }
    );
}

template<>
Type wlo::data::typeOf<Vertex3D>(){
   return  wlo::data::Type(
           "Vertex3D",
           {
                   {"position",data::typeOf<Vec3>()},
           }
   );
}


int main(){


    {
        require(wlo::data::typeOf<Vertex3D>().footprint() == sizeof(Vertex3D));
        auto members = wlo::data::typeOf<Vertex3D>().getMembers();
        require(members[0].offset == offsetof(Vertex3D, position));
    }
    cout<<"got here1"<<endl;
    {
        require(wlo::data::typeOf<TexturedVertex3D>().footprint() == sizeof(TexturedVertex3D));
        auto members = wlo::data::typeOf<TexturedVertex3D>().getMembers();
        require(members[0].offset == offsetof(TexturedVertex3D, position));
        require(members[1].offset == offsetof(TexturedVertex3D, tex));
    }

   auto texvert3D = wlo::data::typeOf<TexturedVertex3D>();
   auto texvert3D2 = wlo::data::typeOf<TexturedVertex3D>();
   require(texvert3D.compatibleWith(texvert3D2));
   require(wlo::data::typeOf<float>().compatibleWith( wlo::data::typeOf<float>()));
   require(wlo::data::typeOf<int>().compatibleWith( wlo::data::typeOf<int>()));
   require(wlo::data::typeOf<unsigned int>().compatibleWith( wlo::data::typeOf<unsigned int>()));

    cout<<"got here3"<<endl;
   require(wlo::data::typeOf<TexturedVertex3D>().compatibleWith(wlo::data::typeOf<ShaderData>()))
   require(wlo::data::typeOf<TexturedVertex3D>()!=wlo::data::typeOf<ShaderData>())

    cout<<"got here4"<<endl;
   require(! wlo::data::typeOf<TexturedVertex3D>().compatibleWith(wlo::data::typeOf<Vertex3D>()))
    require(wlo::data::typeOf<TexturedVertex3D>()!=(wlo::data::typeOf<Vertex3D>()))


    require(wlo::data::typeOf<uint8_t>().footprint()==sizeof(uint8_t));
    require(wlo::data::typeOf<uint8_t>().footprint()==sizeof(char));
    require(wlo::data::typeOf<uint8_t>().footprint()==sizeof(wlo::byte));
    require(wlo::data::typeOf<uint8_t>()==wlo::data::typeOf<uint8_t>());

    cout<<"got here5"<<endl;
}
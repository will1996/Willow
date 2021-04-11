//
// Created by W on 12/5/20.
//
#include"include/willow/data/Type.hpp"
#include"willow/rendering/RenderDataTypes.hpp"
#include<iostream>
using namespace wlo;
using namespace wlo::data;
using namespace std;
#include"TestingTools.hpp"
struct ShaderData{
    Vec3 position;
    Vec2 lightDirection;
};
template<>
wlo::data::Type wlo::data::Type::of<ShaderData>(){
    return wlo::data::Type(
            "ShaderData",
            {
                    {"",Type::of<Vec3>()},
                    {"",Type::of<Vec2>()}
            }
            );
}

int main(){
    {
        require(wlo::data::Type::of<Vertex3D>().footprint() == sizeof(Vertex3D));
        auto members = wlo::data::Type::of<Vertex3D>().getMembers();
        require(members[0].offset == offsetof(Vertex3D, position));
    }

    {
        require(wlo::data::Type::of<TexturedVertex3D>().footprint() == sizeof(TexturedVertex3D));
        auto members = wlo::data::Type::of<TexturedVertex3D>().getMembers();
        require(members[0].offset == offsetof(TexturedVertex3D, position));
        require(members[1].offset == offsetof(TexturedVertex3D, TexCoord));
    }
   require(Type::of<TexturedVertex3D>().compatibleWith( Type::of<TexturedVertex3D>()));
   require(Type::of<float>().compatibleWith( Type::of<float>()));
   require(Type::of<int>().compatibleWith( Type::of<int>()));
   require(Type::of<unsigned int>().compatibleWith( Type::of<unsigned int>()));

   require(Type::of<TexturedVertex3D>().compatibleWith(Type::of<ShaderData>()))
   require(Type::of<TexturedVertex3D>()!=Type::of<ShaderData>())

   require(not Type::of<TexturedVertex3D>().compatibleWith(Type::of<Vertex3D>()))
    require(Type::of<TexturedVertex3D>()!=(Type::of<Vertex3D>()))



}
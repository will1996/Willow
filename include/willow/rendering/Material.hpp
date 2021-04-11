#ifndef WILLOW_MATERIAL_HPP
#define WILLOW_MATERIAL_HPP
#include "willow/root/Root.hpp"
#include "willow/root/Tag.hpp"
#include "willow/root/FileSystem.hpp"
namespace wlo::rendering {

    struct Material : Tag {
        std::string vertexShader;
        std::string fragmentShader;
        std::string texture;
    };
}
inline bool operator==(const wlo::rendering::Material& lhs, const wlo::rendering::Material&rhs){
    return lhs.vertexShader==rhs.vertexShader
           && lhs.fragmentShader==rhs.fragmentShader
           && lhs.texture==rhs.texture
            ;
}

inline std::ostream& operator<<(std::ostream & o, const wlo::rendering::Material&  material){
    o<<"Material: id "<<material.id<<std::endl;
    o<<"\tVertexShader: "<<material.vertexShader<<std::endl;
    o<<"\tFragmentShader: "<<material.fragmentShader<<std::endl;
    o<<"\tTexture: "<<material.texture<<std::endl;
}

namespace std{
    template<>
    struct equal_to<wlo::rendering::Material>
    {

        bool operator()(const wlo::rendering::Material& lhs,const wlo::rendering::Material& rhs) const
        {
            return lhs.vertexShader==rhs.vertexShader
            && lhs.fragmentShader==rhs.fragmentShader
            && lhs.texture==rhs.texture
            ;
        }
    };

    template <>
    struct hash<wlo::rendering::Material>
    {
        std::size_t operator()(const wlo::rendering::Material& mat) const
        {
            using std::size_t;
            using std::hash;
            using std::string;

            // Compute individual hash values for first,
            // second and third and combine them using XOR
            // and bit shifting:
            return hash<string>()(mat.vertexShader)
                    ^ ((hash<string>()(mat.fragmentShader)>>1)<<1)
                        ^ ((hash<string>()(mat.texture)>>2)<<2);
        }
    };

}
#endif

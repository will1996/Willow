//
// Created by W on 10/28/20.
//

#ifndef WILLOW_RENDER_DATA_TYPES_HPP
#define WILLOW_RENDER_DATA_TYPES_HPP
#include "willow/rendering/DataLayout.hpp"

#include<glm/glm.hpp>

#include "glm/gtx/string_cast.hpp"
namespace wlo {
    struct Color{
        glm::vec4 color;
        static rendering::DataLayout Layout() {
            return rendering::DataLayout({
                {rendering::DataLayout::DataType::Float,4}
                                         });
        }
    };

    struct Vertex2D{
        glm::vec2 position;
        static rendering::DataLayout Layout() {
           return rendering::DataLayout({
               {rendering::DataLayout::DataType::Float,2}
           });
        }
    };
    using fVec2 = Vertex2D;

    struct ColorVertex2D: Vertex2D, Color{
        static rendering::DataLayout Layout() {
            return rendering::DataLayout({Vertex2D::Layout(),Color::Layout()});
        }
    };

    struct Vertex3D{
        glm::vec3 position;
        static rendering::DataLayout Layout() {
            return rendering::DataLayout({
               {rendering::DataLayout::DataType::Float,3}
                                       });
        }
    };
    using fVec3 = Vertex3D;

    struct ColorVertex3D{
        glm::vec3 position;
        glm::vec4 color;
        static rendering::DataLayout Layout() {
            return rendering::DataLayout({Vertex3D::Layout(),Color::Layout()});
        }
    };

    using Index = uint32_t;

    struct TexturedVertex3D{
        glm::vec3 position;
        glm::vec2 TexCoord;
        static rendering::DataLayout Layout() {
            return rendering::DataLayout({Vertex3D::Layout(),Vertex2D::Layout()});
        }
    };
   

}

inline    std::ostream& operator <<(std::ostream& os, const wlo::Vertex3D& vert){
    os<<"Vertex {\n"
      <<"Position: "<<glm::to_string(vert.position)<<"\n"
      <<"}";
    return os;
}

template <typename T>
const wlo::rendering::DataLayout Layout(){
    static_assert(std::is_fundamental<T>(),"Failed to find layout for the specified type");
    return wlo::rendering::DataLayout::fromFundamental<T>();
}
template<>
inline const wlo::rendering::DataLayout Layout<wlo::Vertex3D>(){return wlo::Vertex3D::Layout(); };
template<>
inline const wlo::rendering::DataLayout Layout<wlo::Vertex2D>(){return wlo::Vertex2D::Layout(); };
template<>
inline const wlo::rendering::DataLayout Layout<wlo::ColorVertex2D>(){return wlo::ColorVertex2D::Layout(); };
template<>
inline const wlo::rendering::DataLayout Layout<wlo::ColorVertex3D>(){return wlo::ColorVertex3D::Layout(); };
template<>
inline const wlo::rendering::DataLayout Layout<wlo::TexturedVertex3D>(){return wlo::TexturedVertex3D::Layout(); };

template<>
inline const wlo::rendering::DataLayout Layout<glm::mat4x4>(){return wlo::rendering::DataLayout{
            {wlo::rendering::DataLayout::DataType::Float,16}
                                                                }; };
template<>
inline const wlo::rendering::DataLayout Layout<glm::vec4>(){return wlo::rendering::DataLayout{
            {wlo::rendering::DataLayout::DataType::Float,4}
    }; };

template<>
inline const wlo::rendering::DataLayout Layout<wlo::Index>(){return wlo::rendering::DataLayout{
            {wlo::rendering::DataLayout::DataType::Int,1}
    }; };
#endif //WILLOW_RENDER_DATA_TYPES_HPP

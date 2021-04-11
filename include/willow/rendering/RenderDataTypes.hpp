//
// Created by W on 10/28/20.
//

#ifndef WILLOW_RENDER_DATA_TYPES_HPP
#define WILLOW_RENDER_DATA_TYPES_HPP
#include "willow/data/Type.hpp"


namespace wlo {

    struct Color{
        glm::vec4 color;
    };

    using BitColor = glm::vec<4,byte>;

    struct Vertex2D{
        Vec2 position;
    };

    struct Vertex3D{
        Vec3 position;
    };

    struct ColorVertex2D{
        Vec2 position;
        Vec3 color;
    };


    struct ColorVertex3D{
        Vec3 position;
        Vec4 color;
    };

    using Index = uint32_t;

    struct TexturedVertex3D{
        Vec3 position;
        Vec2 TexCoord;
    };
   

}

template<>
inline wlo::data::Type wlo::data::Type::of<wlo::Color>(){
    return wlo::data::Type("Color",{
            {"r",wlo::data::Type::of<float>()},
            {"g",wlo::data::Type::of<float>()},
            {"b",wlo::data::Type::of<float>()},
            {"a",wlo::data::Type::of<float>()}
    });
}

template<>
inline wlo::data::Type wlo::data::Type::of<wlo::BitColor>(){
    return wlo::data::Type("Color",{
            {"r",wlo::data::Type::of<wlo::byte>()},
            {"g",wlo::data::Type::of<wlo::byte>()},
            {"b",wlo::data::Type::of<wlo::byte>()},
            {"a",wlo::data::Type::of<wlo::byte>()}
    });
}
template<>
inline wlo::data::Type wlo::data::Type::of<wlo::Vertex2D>(){
    return wlo::data::Type("Vertex2D",{
            {"position",wlo::data::Type::of<wlo::Vec2>()},
    });
}

template<>
inline wlo::data::Type wlo::data::Type::of<wlo::Vertex3D>(){
    return wlo::data::Type("Vertex3D",{
            {"position",wlo::data::Type::of<wlo::Vec3>()},
    });
}

template<>
inline wlo::data::Type wlo::data::Type::of<wlo::ColorVertex3D>(){
    return wlo::data::Type("Vertex3D",{
            {"position",wlo::data::Type::of<wlo::Vec3>()},
            {"color",wlo::data::Type::of<wlo::Color>()},
    });
}

template<>
inline wlo::data::Type wlo::data::Type::of<wlo::TexturedVertex3D>(){
    return wlo::data::Type("Vertex3D",{
            {"position",wlo::data::Type::of<wlo::Vec3>()},
            {"TexCoord",wlo::data::Type::of<wlo::Vec2>()},
    });
}


inline    std::ostream& operator <<(std::ostream& os, const wlo::Vertex3D& vert){
    os<<"Vertex {\n"
      <<"Position: "<<glm::to_string(vert.position)<<"\n"
      <<"}";
    return os;
}

#endif //WILLOW_RENDER_DATA_TYPES_HPP

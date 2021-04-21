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

    using Pixel1 = uint8_t;
    using Pixel2 = glm::vec<2,uint8_t>;
    using Pixel3 = glm::vec<3,uint8_t>;
    using Pixel4 = glm::vec<4,uint8_t>;

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

template<>
inline wlo::data::Type wlo::data::Type::of<uint8_t>(){
    return wlo::data::Type("uint8_t",sizeof(uint8_t) );
}

template<>
inline wlo::data::Type wlo::data::Type::of<wlo::Pixel2>(){
    return wlo::data::Type("Pixel2",{
            {"r",wlo::data::Type::of<wlo::Pixel1>()},
            {"g",wlo::data::Type::of<wlo::Pixel1>()},
    });
}

template<>
inline wlo::data::Type wlo::data::Type::of<wlo::Pixel3>(){
    return wlo::data::Type("Pixel3",{
            {"r",wlo::data::Type::of<wlo::Pixel1>()},
            {"g",wlo::data::Type::of<wlo::Pixel1>()},
            {"b",wlo::data::Type::of<wlo::Pixel1>()},
    });
}

template<>
inline wlo::data::Type wlo::data::Type::of<wlo::Pixel4>(){
    return wlo::data::Type("Pixel4",{
            {"r",wlo::data::Type::of<wlo::Pixel1>()},
            {"g",wlo::data::Type::of<wlo::Pixel1>()},
            {"b",wlo::data::Type::of<wlo::Pixel1>()},
            {"a",wlo::data::Type::of<wlo::Pixel1>()},
    });
}
inline    std::ostream& operator <<(std::ostream& os, const wlo::Vertex3D& vert){
    os<<"Vertex {\n"
      <<"Position: "<<glm::to_string(vert.position)<<"\n"
      <<"}";
    return os;
}

#endif //WILLOW_RENDER_DATA_TYPES_HPP

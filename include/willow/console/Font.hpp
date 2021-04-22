//
// Created by W on 10/9/20.
//

#ifndef WILLOW_FONT_HPP
#define WILLOW_FONT_HPP
#include"willow/root/Root.hpp"
#include"willow/rendering/RenderDataTypes.hpp"
#include "willow/input/KeyCodes.hpp"
#include"willow/rendering/Texture.hpp"
#include<array>



namespace wlo {
    class Font {
    public:
        struct Glyph{
            char textValue;
            size_t width;
            size_t height;
            std::array<wlo::TexturedVertex3D,4> verts;
            constexpr static std::array<wlo::Index,6 > inds = {0,1,2,2,3,0};
        };

        Font(std::string ttfFile);

        const Glyph & operator [](wlo::Key::Code)const;
    private:
        std::unordered_map<wlo::Key::Code, Glyph> m_fontAtlas;
        Texture m_BitMap;
    };
}


#endif //WILLOW_FONT_HPP

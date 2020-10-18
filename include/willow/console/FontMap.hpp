//
// Created by W on 10/9/20.
//

#ifndef WILLOW_FONTMAP_HPP
#define WILLOW_FONTMAP_HPP
#include"willow/root/wilo_dev_core.hpp"
#include "willow/rendering/wilo_vertex_types.hpp"
#include "willow/input/KeyCodes.hpp"
namespace wlo {
    class FontMap {
    public:
        struct Character{
            char textValue;
            std::vector<wlo::Vertex3D> verts;
            std::vector<uint32_t > inds;
        };

        FontMap();
        std::unordered_map<wlo::Key::Code,Character> get(size_t font_height, size_t font_width);
    private:
        struct LineSegment{
            glm::vec2 start;
            glm::vec2 end;
        };
        Character build_Q();
        Character build_0();
        std::unordered_map<wlo::Key::Code, Character> m_fontMap;
         std::pair<std::vector<wlo::Vertex3D> , std::vector<uint32_t> > segmentToTri(LineSegment);
    };
}


#endif //WILLOW_FONTMAP_HPP

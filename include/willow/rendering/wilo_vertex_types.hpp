#pragma once
#include "wilo_render_data_layout.hpp"


namespace wlo {
    //basic defintions of vertex data types, this list will grow as it makes sense to

    struct Vertex2D {
        glm::vec2 position;
        glm::vec3 color;
        static const RenderDataLayout Layout() {
            const RenderDataLayout layout = {
                {RenderDataType::fvec2, "position"},
                {RenderDataType::fvec3, "color"}
            };
            return layout;
        }
    };

    struct Vertex3D {
        glm::vec3 position;
        glm::vec3 color;

        static const RenderDataLayout Layout() {
            const RenderDataLayout layout{
                {RenderDataType::fvec3, "position"},
                {RenderDataType::fvec3, "color"}
            };
            return layout;
        }
    };
}

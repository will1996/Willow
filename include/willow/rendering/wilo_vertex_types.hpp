#pragma once
#include "wilo_render_data_layout.hpp"
#include "willow/root/wilo_dev_core.hpp"
#include "glm/gtx/string_cast.hpp"
#include<iostream>

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
        static Vertex3D setColor(const Vertex3D & vert, glm::vec3 color){
            Vertex3D outVert = vert;
            outVert.color = color;
            return outVert;
        }
    };

inline    std::ostream& operator <<(std::ostream& os, const Vertex3D& vert){
        os<<"Vertex {\n"
            <<"Position: "<<glm::to_string(vert.position)<<"\n"
            <<"Color: " << glm::to_string(vert.color)<<"\n"
            <<"}";
        return os;
    }

inline Vertex3D operator *(const glm::mat4x4 & mat, const Vertex3D & vert){
        return Vertex3D{mat*glm::vec4(vert.position,1),vert.color};
    }

}

//
// Created by W on 10/28/20.
//

#ifndef WILLOW_RENDER_DATA_TYPES_HPP
#define WILLOW_RENDER_DATA_TYPES_HPP
#include "willow/root/wilo_dev_core.hpp"

#include<glm/glm.hpp>

namespace wlo {
    namespace gfx{
        struct LayoutElement{
           size_t size;
           size_t offset;
        };



    template<class ... Bases>
    struct Vertex : public Bases ... {
    constexpr static std::array<LayoutElement,sizeof...(Bases)> DataLayout(){
        std::array<LayoutElement,sizeof...(Bases)> dataLayout;
        size_t i =0;
        size_t offset  =0;
        (void(dataLayout[i++] = {sizeof(Bases),  [&offset](size_t size ){size_t old = offset; offset+= size; return old;}(sizeof(Bases))}),...);

        return dataLayout;
    }
    };


    struct posWrap{
        glm::vec3 position;
    };
    struct colorWrap{
        glm::vec4 color;
    };


    using ColorVert3D = Vertex<posWrap,colorWrap> ;


    }

}


#endif //WILLOW_RENDER_DATA_TYPES_HPP

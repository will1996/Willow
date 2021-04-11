//
// Created by W on 12/9/20.
//

#ifndef WILLOW_VIEW_HPP
#define WILLOW_VIEW_HPP
#include "Type.hpp"
#include"willow/rendering/RenderDataTypes.hpp"
#include "willow/root/Tag.hpp"
namespace wlo {

    struct View {
        const byte *source;
        const wlo::data::Type layout;
        const size_t count;
        const size_t memSize;

        View():
        source(nullptr),
        count(0),
        memSize(0)
        {};

        template<typename T>
        explicit View(const std::vector<T> &resource):
                source(reinterpret_cast<const byte*>(resource.data())), layout(wlo::data::Type::of<T>()), count(resource.size()),
                memSize(count * layout.footprint()) {}

        template<typename T>
        View(const std::vector<T> &resource, size_t offset, size_t count):
                source(&resource.data()[offset]), layout(Layout<T>()), count(count),
                memSize(count * layout.footprint()) {}

        inline View(byte *data, wlo::data::Type layout, size_t count) : source(data), layout(layout), count(count),
                                                                        memSize(count * layout.footprint()) {}


    };
}

#include"iostream"



#endif //WILLOW_VIEW_HPP

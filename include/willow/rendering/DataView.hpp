//
// Created by W on 12/9/20.
//

#ifndef WILLOW_DATAVIEW_HPP
#define WILLOW_DATAVIEW_HPP
#include "DataLayout.hpp"
#include"RenderDataTypes.hpp"
#include "Tag.hpp"
namespace wlo::rendering {

    struct DataView {
        const byte *source;
        const DataLayout layout;
        const size_t count;
        const size_t memSize;

        template<typename T>
        explicit DataView(const std::vector<T> &resource):
                source(reinterpret_cast<const byte*>(resource.data())), layout(Layout<T>()), count(resource.size()),
                memSize(count * layout.memSize()) {}

        template<typename T>
        DataView(const std::vector<T> &resource, size_t offset, size_t count):
                source(&resource.data()[offset]), layout(Layout<T>()), count(count),
                memSize(count * layout.memSize()) {}

        inline DataView(byte *data, DataLayout layout, size_t count) : source(data), layout(layout), count(count),
                                                                       memSize(count * layout.memSize()) {}


    };
}

#include<iostream>



#endif //WILLOW_DATAVIEW_HPP

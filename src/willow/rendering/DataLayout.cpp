//
// Created by W on 12/6/20.
//
#include"willow/rendering/DataLayout.hpp"

namespace wlo::rendering{
   DataLayout::DataLayout(std::initializer_list<Element> elements):m_memSize(0) {
       size_t offset =0;
        for(auto element : elements) {
            m_elementList.push_back({element, offset});
            m_memSize+=sizeOf(element);
            offset+=sizeOf(element);
        }
   }
   DataLayout::DataLayout(std::initializer_list<const DataLayout> layouts):m_memSize(0){
       size_t offset =0;
        for(auto layout: layouts)
            for(auto [element, _] : layout.m_elementList) {
                m_elementList.push_back({element, offset});
                m_memSize+=sizeOf(element);
                offset+=sizeOf(element);
            }

   }

    size_t DataLayout::sizeOf(Element el){
        return  el.count*sizeOf(el.type);
   }
    size_t  DataLayout::sizeOf(DataType type){
        switch (type) {
            case wlo::rendering::DataLayout::DataType::Float:
                return sizeof(float);
            case wlo::rendering::DataLayout::DataType::Double:
                return sizeof(double);
            case wlo::rendering::DataLayout::DataType::Int:
                return sizeof(int);
            case wlo::rendering::DataLayout::DataType::Bool:
                return sizeof(bool);
            case wlo::rendering::DataLayout::DataType::Long:
                return sizeof(uint64_t);
        }
   }

   size_t DataLayout::memSize() const {return m_memSize;}

   std::vector<std::pair<DataLayout::Element, size_t>> DataLayout::get() const {
       return m_elementList;
   }

}




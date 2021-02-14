//
// Created by W on 1/18/21.
//

#ifndef WILLOW_MODEL_HPP
#define WILLOW_MODEL_HPP
#include"willow/root/Root.hpp"
#include "willow/root/Tag.hpp"
#include"willow/rendering/DataLayout.hpp"
#include"willow/rendering/RenderDataTypes.hpp"
#include "willow/rendering/Material.hpp"
namespace wlo::rendering{

template<typename T>
struct Model:Tag{
    vector<T> vertices;
    vector<Index> indices;
    Material material;
};
}


#endif //WILLOW_MODEL_HPP

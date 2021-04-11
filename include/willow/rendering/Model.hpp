//
// Created by W on 1/18/21.
//

#ifndef WILLOW_MODEL_HPP
#define WILLOW_MODEL_HPP
#include"willow/root/Root.hpp"
#include "willow/root/Tag.hpp"
#include"include/willow/data/Type.hpp"
#include"willow/rendering/RenderDataTypes.hpp"
#include"willow/rendering/Mesh.hpp"
#include "willow/rendering/Material.hpp"
#include "include/willow/data/View.hpp"
namespace wlo::rendering{

template<typename T>
struct Model:Tag{
    Mesh<T> mesh;
    Material material;
};

struct ModelView:Tag{
    View vertices;
    View indices;
    Material  material;

    template<typename T>
    explicit ModelView(Model<T> model):vertices(model.mesh.vertices),indices(model.mesh.indices),material(model.material){}

};


}


#endif //WILLOW_MODEL_HPP

//
// Created by W on 1/18/21.
//

#ifndef WILLOW_MODEL_HPP
#define WILLOW_MODEL_HPP
#include "willow/rendering/Tag.hpp"
#include"willow/rendering/DataLayout.hpp"
#include"willow/root/wilo_dev_core.hpp"
#include"willow/rendering/RenderDataTypes.hpp"
#include"willow/rendering/Frame.hpp"
#include "willow/rendering/RenderPath.hpp"
namespace wlo::rendering{
struct Model3D:Tag{
    vector<TexturedVertex3D> vertices;
    vector<Index> indices;
    glm::mat4x4 transform;
    std::string textureFile = "";

    Frame::TransformedGeometry draw();
    static Model3D fromFile(std::string file);

};
}


#endif //WILLOW_MODEL_HPP

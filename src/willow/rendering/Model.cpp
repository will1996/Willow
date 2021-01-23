//
// Created by W on 1/18/21.
//

#include "willow/rendering/Model.hpp"
namespace wlo::rendering {
    Model3D wlo::rendering::Model3D::fromFile(std::string file) {
        throw std::runtime_error("Model3D::fromFile unimplemented");
    }

    Frame:: TransformedGeometry Model3D::draw() {
        return  Frame::TransformedGeometry{
                .vertices = DataView(vertices),
                .indices = DataView(indices),
                .ModelMatrix = transform,
                .texturesPaths = {textureFile}
        };
    }
}

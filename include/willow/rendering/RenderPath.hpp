//
// Created by W on 10/29/20.
//

#ifndef WILLOW_RENDERPATH_HPP
#define WILLOW_RENDERPATH_HPP
#include"RenderDataTypes.hpp"
#include"willow/window/window.hpp"
#include "OrthographicCamera3D.hpp"
#include"DataView.hpp"
#include"Tag.hpp"

namespace wlo::rendering {
    struct RenderPath:Tag {
        public:
        std::pair<DataLayout,size_t> vertexInputDescription;
        OrthographicCamera3D camera;
        std::string vertexShaderPath;
        std::string fragmentShaderPath;
        bool depthBuffer = true;
        std::pair<float,float> viewport = {-1,-1};
    };



}


#endif //WILLOW_RENDERPATH_HPP

//
// Created by W on 10/29/20.
//

#ifndef WILLOW_RENDERPATH_HPP
#define WILLOW_RENDERPATH_HPP
#include"RenderDataTypes.hpp"
#include"willow/window/window.hpp"
#include "PrespectiveCamera3D.hpp"
#include"DataView.hpp"
#include"Tag.hpp"

namespace wlo::rendering {
    struct RenderPath:Tag {
        public:
        PrespectiveCamera3D *camera;
        std::string vertexShaderPath;
        std::string fragmentShaderPath;
        std::string texturePath = "";
    };


}


#endif //WILLOW_RENDERPATH_HPP

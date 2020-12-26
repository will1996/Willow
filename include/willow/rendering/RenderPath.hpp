//
// Created by W on 10/29/20.
//

#ifndef WILLOW_RENDERPATH_HPP
#define WILLOW_RENDERPATH_HPP
#include"RenderDataTypes.hpp"
#include"willow/window/window.hpp"
#include "willow/rendering/Shader.hpp"
#include "OrthographicCamera3D.hpp"
#include"Attachment.hpp"
#include"Tag.hpp"

namespace wlo::rendering {
    struct RenderPath:Tag {
        public:
        std::vector<Attachment> attachments = {};
        OrthographicCamera3D camera;
        SharedPointer<VertexShader> vertexShader;
        SharedPointer<FragmentShader> fragmentShader;
        bool depthBuffer = true;
        std::pair<float,float> viewport = {-1,-1};
    };



}


#endif //WILLOW_RENDERPATH_HPP

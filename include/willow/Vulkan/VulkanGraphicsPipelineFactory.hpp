//
// Created by W on 12/26/20.
//

#ifndef WILLOW_VULKANGRAPHICSPIPELINEFACTORY_HPP
#define WILLOW_VULKANGRAPHICSPIPELINEFACTORY_HPP

#include"VulkanRoot.hpp"
#include "willow/rendering/RenderPath.hpp"
namespace wlo::wk {
    class VulkanGraphicsPipelineFactory {
    public:
        VulkanGraphicsPipelineFactory(VulkanRoot & root);
        vk::Pipeline make(wlo::rendering::RenderPath);
    private:
        VulkanRoot & m_root;
    };
}
#endif //WILLOW_VULKANGRAPHICSPIPELINEFACTORY_HPP

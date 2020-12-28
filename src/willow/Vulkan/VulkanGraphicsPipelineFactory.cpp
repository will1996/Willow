//
// Created by W on 12/26/20.
//

#include <willow/Vulkan/VulkanGraphicsPipelineFactory.hpp>

wlo::wk::VulkanGraphicsPipelineFactory::VulkanGraphicsPipelineFactory(VulkanRoot& root):
m_root(root)
{
}

vk::Pipeline wlo::wk::VulkanGraphicsPipelineFactory::make(wlo::rendering::RenderPath) {
    return nullptr;
}


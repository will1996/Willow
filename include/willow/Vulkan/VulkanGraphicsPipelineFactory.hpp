//
// Created by W on 12/26/20.
//

#ifndef WILLOW_VULKANGRAPHICSPIPELINEFACTORY_HPP
#define WILLOW_VULKANGRAPHICSPIPELINEFACTORY_HPP

#include"VulkanRoot.hpp"
#include "willow/rendering/RenderPath.hpp"
#include "willow/Vulkan/VulkanShaderCompiler.hpp"
#include "willow/Vulkan/VulkanSwapchain.hpp"
namespace wlo::wk {
    struct GraphicsPipeline {
        wlo::ID_type id;
        wlo::rendering::RenderPath originPath;
        wlo::rendering::DataLayout vertexLayout;
        wlo::rendering::DataLayout uniformBufferLayout;
        wlo::rendering::DataLayout pushConstantLayout;
        vk::UniquePipelineLayout vkPipelineLayout;
        vk::UniquePipeline vkPipeline;
        vk::UniqueRenderPass vkRenderPass;
        vk::UniqueDescriptorSetLayout  vkDescriptorSetLayout;
    };
    class VulkanGraphicsPipelineFactory {
    public:
        VulkanGraphicsPipelineFactory(VulkanRoot & root,VulkanShaderCompiler & compiler,VulkanSwapchain &);
        GraphicsPipeline buildGraphicsPipeline( wlo::rendering::RenderPath);
        void rebuildGraphicsPipeline(GraphicsPipeline &);
    private:
        vk::UniqueDescriptorSetLayout createDescriptorSetLayout(wlo::rendering::RenderPath);
        vk::UniquePipelineLayout createPipelineLayout(wlo::rendering::RenderPath,
                                                      vk::UniqueDescriptorSetLayout &descriptorSetLayout,
                                                      wlo::rendering::DataLayout pushLayout);
        vk::UniqueRenderPass createRenderPass(wlo::rendering::RenderPath);
        vk::UniquePipeline  createPipeline(wlo::rendering::RenderPath,
                                                   vk::UniqueRenderPass &,
                                                   vk::UniquePipelineLayout & );
        VulkanRoot & m_root;
        VulkanShaderCompiler & m_shaderCompiler;
        VulkanSwapchain & m_swapchain;


    };
}
#endif //WILLOW_VULKANGRAPHICSPIPELINEFACTORY_HPP

//
// Created by W on 12/26/20.
//

#ifndef WILLOW_VULKANGRAPHICSPIPELINEFACTORY_HPP
#define WILLOW_VULKANGRAPHICSPIPELINEFACTORY_HPP

#include"VulkanRoot.hpp"
#include "willow/Vulkan/VulkanShaderCompiler.hpp"
#include "willow/Vulkan/VulkanSwapchain.hpp"
#include "willow/rendering/Material.hpp"
#include "willow/root/Tag.hpp"

namespace wlo::wk {
    struct GraphicsPipeline {
        wlo::ID_type id;
        wlo::ID_type texID;
        wlo::data::Type vertexLayout;
        wlo::data::Type uniformBufferLayout;
        wlo::data::Type pushConstantLayout;
        vk::UniquePipelineLayout vkPipelineLayout;
        vk::UniquePipeline vkPipeline;
        vk::UniqueDescriptorSetLayout  vkDescriptorSetLayout;
    };
    class VulkanGraphicsPipelineFactory {
    public:
        VulkanGraphicsPipelineFactory(VulkanRoot & root,VulkanShaderCompiler & compiler,VulkanSwapchain &);
        GraphicsPipeline buildGraphicsPipeline( const wlo::rendering::Material & ,vk::UniqueRenderPass& renderPass);
    private:
        vk::UniqueDescriptorSetLayout createDescriptorSetLayout(const wlo::rendering::Material & );
        vk::UniquePipelineLayout createPipelineLayout(const wlo::rendering::Material & ,
                                                      vk::UniqueDescriptorSetLayout &descriptorSetLayout,
                                                      wlo::data::Type pushLayout);
        vk::UniquePipeline  createPipeline(const wlo::rendering::Material & ,
                                                   vk::UniqueRenderPass &,
                                                   vk::UniquePipelineLayout & );
        VulkanRoot & m_root;
        VulkanShaderCompiler & m_shaderCompiler;
        VulkanSwapchain & m_swapchain;


    };
}
#endif //WILLOW_VULKANGRAPHICSPIPELINEFACTORY_HPP

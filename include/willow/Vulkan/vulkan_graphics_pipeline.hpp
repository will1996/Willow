#pragma once
#include"vulkan_context.hpp"
#include "VulkanSwapchain.hpp"
#include "vulkan_shader.hpp"
#include "vulkan_render_pass.hpp"
#include "willow/dev-utils/Vulkan/wilovk.hpp"
#include <tuple>


namespace wlo{
    class VulkanGraphicsPipeline{
        public:
            VulkanGraphicsPipeline(
                VkDevice device,
                VkExtent2D extent,
                const VulkanRenderPass& renderPass,
                VulkanShader vertexShader,
                VulkanShader fragmentShader
        );

        VkPipeline get();
        
        VkPipelineLayout getLayout();
        
        void reinitialize(VkDevice device, VkExtent2D extent);

        void reclaim(VkDevice device) ;

        ~VulkanGraphicsPipeline();


        void pushDescriptorSetLayout(VkDescriptorSetLayout layout);


        private:
        
        void bindVertexShader(VulkanShader vertexStage ) ;
        void bindFragmentShader(VulkanShader fragmentStage ) ;

        VkVertexInputBindingDescription buildVertexInputBindingDescription();

        std::vector<VkVertexInputAttributeDescription> buildVertexInputAttributes();

        VkPipelineVertexInputStateCreateInfo setVertexInputState();
        VkPipelineInputAssemblyStateCreateInfo setVertexAssemblyState();

        VkRect2D buildScissor(VkExtent2D);
        VkViewport buildViewport(VkExtent2D);
        VkPipelineViewportStateCreateInfo setViewportState(VkRect2D* scissor, VkViewport* viewport);

        VkPipelineRasterizationStateCreateInfo setRasterizerState();
        VkPipelineMultisampleStateCreateInfo setMultisamplingState();
        VkPipelineColorBlendAttachmentState setColorAttachmentState();
        VkPipelineColorBlendStateCreateInfo setColorBlendingState();
        VkPipelineDynamicStateCreateInfo setDynamicState();
        VkPipelineLayoutCreateInfo setLayout();
        void createVkPipelineLayout(VkDevice);
        void createVkPipeline(VkDevice , VkExtent2D );

        VkVertexInputBindingDescription m_vertexBindingDescription;
        std::vector<VkVertexInputAttributeDescription> m_vertexAttributeDescriptions;
        RenderDataLayout m_vertexInputDataLayout;
        std::array<VkPipelineShaderStageCreateInfo, 2> m_shaders;
        std::vector<VkDescriptorSetLayout> m_descriptorSetLayouts;
        VkPipelineLayout m_vkPipelineLayout;
        VkPipeline m_vkPipeline;
        VkRenderPass m_vkRenderPass;
    };


}



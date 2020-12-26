#include "willow/rendering/Vulkan/vulkan_graphics_pipeline.hpp"
#include "willow/rendering/Vulkan/vulkan_render_pass.hpp"

namespace wlo{


   static VkFormat formatFromRenderDataType(RenderDataType type){
        switch (type){
        case RenderDataType::f: return VK_FORMAT_R32_SFLOAT ;
        case RenderDataType::fvec2: return VK_FORMAT_R32G32_SFLOAT;
        case RenderDataType::fvec3: return VK_FORMAT_R32G32B32_SFLOAT;
        case RenderDataType::fvec4: return VK_FORMAT_R32G32B32A32_SFLOAT;
        }
        throw std::runtime_error("no vulkan conversion for input type");
   }

		VulkanGraphicsPipeline::VulkanGraphicsPipeline(VkDevice device,
                                                        VkExtent2D extent,
                                                        const VulkanRenderPass& renderPass,
                                                        VulkanShader vertexShader, 
                                                        VulkanShader fragmentShader)
            : m_vertexInputDataLayout(vertexShader.getInputDataLayout())
		{
            bindVertexShader(vertexShader);
            bindFragmentShader(fragmentShader);
            m_vkRenderPass = renderPass.get();
            createVkPipeline(device, extent);

		}

		VulkanGraphicsPipeline::~VulkanGraphicsPipeline() {}

        void VulkanGraphicsPipeline::bindVertexShader(VulkanShader shader){
            assert(shader.getType() == wlo::Shader::Type::Vertex);

            m_shaders[0] = shader.getInfo();
             

        } 

        void VulkanGraphicsPipeline::bindFragmentShader(VulkanShader shader){
            assert(shader.getType() == wlo::Shader::Type::Fragment);
            m_shaders[1] = shader.getInfo();
        } 


		void VulkanGraphicsPipeline::pushDescriptorSetLayout(VkDescriptorSetLayout layout)
		{
            m_descriptorSetLayouts.push_back(layout);
		}

		void VulkanGraphicsPipeline::reclaim(VkDevice device) {
            //Note: shaders were previously destroyed here, I'm hoping that with the shader modules being cleaned up by the reclaim functions on the shaders themselves, this won't be needed
            vkDestroyPipelineLayout(device,m_vkPipelineLayout,nullptr);
            vkDestroyPipeline(device,m_vkPipeline,nullptr);
        }

        void VulkanGraphicsPipeline::reinitialize(VkDevice device, VkExtent2D extent){
            //rebuild pipeline, pulling up-to-date information from the swapchain
            vkDestroyPipelineLayout(device,m_vkPipelineLayout,nullptr);
            vkDestroyPipeline(device,m_vkPipeline,nullptr);
            createVkPipeline(device, extent);
        } ;

        VkVertexInputBindingDescription VulkanGraphicsPipeline::buildVertexInputBindingDescription(){
            VkVertexInputBindingDescription bindingDescription{};
            bindingDescription.binding = 0;
            bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
            bindingDescription.stride = m_vertexInputDataLayout.getTotalSize();
            return bindingDescription;
        }

        std::vector<VkVertexInputAttributeDescription> VulkanGraphicsPipeline::buildVertexInputAttributes(){

            std::vector<VkVertexInputAttributeDescription>    attributes;
            size_t currLocation = 0;
            for (RenderDataLayoutElement element : m_vertexInputDataLayout.getElements()){
                VkVertexInputAttributeDescription atrib;
                atrib.binding = 0;
                atrib.format = formatFromRenderDataType(element.type);
                atrib.location = currLocation;
                atrib.offset = element.offset;
                attributes.push_back(atrib);

                currLocation++;
            }
            return attributes;
        }




        VkPipelineVertexInputStateCreateInfo VulkanGraphicsPipeline::setVertexInputState(){
            VkPipelineVertexInputStateCreateInfo info{};
            info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
            m_vertexBindingDescription= buildVertexInputBindingDescription();
            m_vertexAttributeDescriptions = buildVertexInputAttributes();

            info.vertexBindingDescriptionCount = 1;
            info.pVertexBindingDescriptions = &m_vertexBindingDescription;

            info.vertexAttributeDescriptionCount = static_cast<uint32_t>(m_vertexAttributeDescriptions.size());
            info.pVertexAttributeDescriptions = m_vertexAttributeDescriptions.data();
            return info;
        }
 



       VkPipelineInputAssemblyStateCreateInfo VulkanGraphicsPipeline::setVertexAssemblyState(){
           VkPipelineInputAssemblyStateCreateInfo info{};
            info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
            info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
            info.primitiveRestartEnable = VK_FALSE;
            return info;
        }

	   VkRect2D VulkanGraphicsPipeline::buildScissor(VkExtent2D extent)
	   {

            VkRect2D scissorState;
            scissorState.offset = {0,0};
            scissorState.extent = extent;
            return scissorState;
	   }

	   VkViewport VulkanGraphicsPipeline::buildViewport(VkExtent2D extent)
	   {

            VkViewport viewportState;
            viewportState.x = 0.0f;
            viewportState.y = 0.0f;
            viewportState.width = extent.width;
            viewportState.height = extent.height;

            viewportState.minDepth = 0.0f;
            viewportState.maxDepth = 1.0f;
            return viewportState;
	   }

	   VkPipelineViewportStateCreateInfo VulkanGraphicsPipeline::setViewportState(VkRect2D* p_scissorState, VkViewport* p_viewportState) {
            VkPipelineViewportStateCreateInfo info{};
            info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
            info.viewportCount = 1;
            info.scissorCount = 1;
            info.pScissors = p_scissorState;
            info.pViewports = p_viewportState;

            return info;
        }
        VkPipelineRasterizationStateCreateInfo VulkanGraphicsPipeline::setRasterizerState(){
            VkPipelineRasterizationStateCreateInfo info{};
                info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
                info.depthClampEnable = VK_FALSE;
                info.rasterizerDiscardEnable = VK_FALSE;
                info.polygonMode = VK_POLYGON_MODE_FILL;
                info.lineWidth = 1.0f;
                info.cullMode = VK_CULL_MODE_BACK_BIT;
                info.frontFace = VK_FRONT_FACE_CLOCKWISE;
                info.depthBiasEnable = VK_FALSE;
                //TODO implement depth bias settings
                return info;
        }
        VkPipelineMultisampleStateCreateInfo VulkanGraphicsPipeline::setMultisamplingState(){
            VkPipelineMultisampleStateCreateInfo info{};
            info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
            info.sampleShadingEnable = VK_FALSE;
            info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
            //TODO implement more settings
            return info;

        }
        VkPipelineColorBlendAttachmentState VulkanGraphicsPipeline::setColorAttachmentState(){
            VkPipelineColorBlendAttachmentState colorBlendAttachmentState{};
                colorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT|
                                            VK_COLOR_COMPONENT_G_BIT|VK_COLOR_COMPONENT_B_BIT|VK_COLOR_COMPONENT_A_BIT;
                colorBlendAttachmentState.blendEnable = VK_TRUE;

                colorBlendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
                colorBlendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
                colorBlendAttachmentState.colorBlendOp  = VK_BLEND_OP_ADD;

                colorBlendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
                colorBlendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
                colorBlendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;
                return colorBlendAttachmentState;
        }
        VkPipelineColorBlendStateCreateInfo VulkanGraphicsPipeline::setColorBlendingState(){
                VkPipelineColorBlendStateCreateInfo info{};
                info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;


                info.logicOpEnable = VK_FALSE;
                info.logicOp = VK_LOGIC_OP_COPY;
                
                info.attachmentCount = 1;
                //TODO add settings for blend constants
                return info;
        }
        VkPipelineDynamicStateCreateInfo VulkanGraphicsPipeline::setDynamicState(){
            VkPipelineDynamicStateCreateInfo info{};
            return info;
               //TODO add dynamic state, presently all graphics pipelines are fully immutable 
        }

        VkPipelineLayoutCreateInfo VulkanGraphicsPipeline::setLayout(){
            VkPipelineLayoutCreateInfo info{};
            info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
            info.setLayoutCount = m_descriptorSetLayouts.size();

            if (m_descriptorSetLayouts.size() > 0)
                info.pSetLayouts = m_descriptorSetLayouts.data();
            else
                info.pSetLayouts = nullptr;
            
            return info;

        }
        void VulkanGraphicsPipeline::createVkPipelineLayout(VkDevice device){
            auto layoutInfo = setLayout();
            VkResult res = vkCreatePipelineLayout(device,&layoutInfo, nullptr, &m_vkPipelineLayout);
            vk::checkResult(res,"create graphics pipeline layout");
        }




        void VulkanGraphicsPipeline::createVkPipeline(VkDevice device, VkExtent2D extent){
            VkGraphicsPipelineCreateInfo pipelineInfo{};
            pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

            VkPipelineVertexInputStateCreateInfo inputInfo = setVertexInputState();
            pipelineInfo.pVertexInputState = &inputInfo;

            VkPipelineInputAssemblyStateCreateInfo assemblyInfo =  setVertexAssemblyState();
            pipelineInfo.pInputAssemblyState = &assemblyInfo;

            VkRect2D scissorState = buildScissor(extent);
            VkViewport viewport = buildViewport(extent);
            VkPipelineViewportStateCreateInfo viewportInfo = setViewportState(&scissorState, &viewport);

            pipelineInfo.pViewportState = &viewportInfo;

            VkPipelineRasterizationStateCreateInfo rasterizationInfo  = setRasterizerState();
            pipelineInfo.pRasterizationState = &rasterizationInfo;

            VkPipelineMultisampleStateCreateInfo multisamplingInfo =  setMultisamplingState();
            pipelineInfo.pMultisampleState = &multisamplingInfo;



            VkPipelineColorBlendStateCreateInfo colorBlendInfo = setColorBlendingState();
            pipelineInfo.pColorBlendState = &colorBlendInfo;

            VkPipelineColorBlendAttachmentState colorBlendAttachmentState = setColorAttachmentState();
            colorBlendInfo.pAttachments = &colorBlendAttachmentState;

            pipelineInfo.stageCount =  2;
            pipelineInfo.pStages = m_shaders.data();


            //dynamic state is nullptr, meaning there is no dymnamic state!
            pipelineInfo.pDynamicState = nullptr; 

            pipelineInfo.renderPass = m_vkRenderPass;
            pipelineInfo.subpass = 0;

            createVkPipelineLayout(device);
            pipelineInfo.layout = m_vkPipelineLayout;

            pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
            pipelineInfo.basePipelineIndex = -1;

            VkResult res = vkCreateGraphicsPipelines(device,VK_NULL_HANDLE,1, &pipelineInfo,nullptr, &m_vkPipeline);
            vk::checkResult(res,"create graphics pipeline");
            
 
        }


        VkPipeline VulkanGraphicsPipeline::get(){
            return m_vkPipeline;
        }

VkPipelineLayout VulkanGraphicsPipeline::getLayout(){
    return m_vkPipelineLayout;
}

}


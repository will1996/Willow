//
// Created by W on 12/26/20.
//

#include <willow/Vulkan/VulkanGraphicsPipelineFactory.hpp>
using namespace wlo::rendering;
namespace wlo::wk {
   VulkanGraphicsPipelineFactory::VulkanGraphicsPipelineFactory(VulkanRoot &root,
                                                                VulkanShaderCompiler &compiler,
                                                                VulkanSwapchain &swapchain) :
            m_root(root),
            m_shaderCompiler(compiler),
            m_swapchain(swapchain)
            { }


    std::vector<vk::VertexInputAttributeDescription> tovkVertexAttributes(const wlo::rendering::DataLayout&  layout) {
            std::vector<vk::VertexInputAttributeDescription> desc;
            uint32_t bindingLocationCounter = 0;
            for(auto & [element,offset] : layout.get()){
                switch(element.type){
                    case wlo::rendering::DataLayout::DataType::Float:
                        if(element.count==1)
                            desc.push_back(vk::VertexInputAttributeDescription(bindingLocationCounter,0,vk::Format::eD32Sfloat,offset));
                        else if(element.count==2)
                            desc.push_back(vk::VertexInputAttributeDescription(bindingLocationCounter,0,vk::Format::eR32G32Sfloat,offset));
                        else if(element.count==3)
                            desc.push_back(vk::VertexInputAttributeDescription(bindingLocationCounter,0,vk::Format::eR32G32B32Sfloat,offset));
                        else if(element.count==4)
                            desc.push_back(vk::VertexInputAttributeDescription(bindingLocationCounter,0,vk::Format::eR32G32B32A32Sfloat,offset));
                        else
                            throw std::runtime_error("Vertex Input description for Data Layout element type FLOAT with count greater than four unsupported");
                        break;
                }
                bindingLocationCounter++;
            }
            return desc;
   }

    vk::UniquePipelineLayout VulkanGraphicsPipelineFactory::createPipelineLayout(wlo::rendering::RenderPath,
                                                                                 vk::UniqueDescriptorSetLayout &descriptorSetLayout,
                                                                                 wlo::rendering::DataLayout pushLayout) {


        std::vector<vk::PushConstantRange> pushConstantRanges = {
                vk::PushConstantRange(vk::ShaderStageFlagBits::eVertex,0,pushLayout.memSize())
        };

        // create a PipelineLayout using that DescriptorSetLayout
       return m_root.Device().createPipelineLayoutUnique(
                vk::PipelineLayoutCreateInfo(vk::PipelineLayoutCreateFlags(), descriptorSetLayout.get(), pushConstantRanges));

    }

    wlo::wk::GraphicsPipeline
    wlo::wk::VulkanGraphicsPipelineFactory::buildGraphicsPipeline(wlo::rendering::RenderPath path) {

        VulkanShader vertexShader = m_shaderCompiler.ShaderFromBinary(vk::ShaderStageFlagBits::eVertex,path.vertexShaderPath);
        VulkanShader fragmentShader = m_shaderCompiler.ShaderFromBinary(vk::ShaderStageFlagBits::eFragment,path.fragmentShaderPath);

        DataLayout vertexLayout = vertexShader.vertexInputLayout();
        DataLayout uniformLayout = vertexShader.uniformBufferLayout();
        DataLayout pushLayout = vertexShader.pushConstantLayout();

        auto descriptorSetLayout = createDescriptorSetLayout(path);
        auto pipelineLayout = createPipelineLayout(path,descriptorSetLayout,pushLayout);
        auto renderPass = createRenderPass(path);
        auto pipeline = createPipeline(path,renderPass,pipelineLayout);
        return GraphicsPipeline{
            .originPath = path,
           .vertexLayout = vertexLayout,
           .uniformBufferLayout = uniformLayout,
           .pushConstantLayout = pushLayout,
           .id = path.id,
           .vkDescriptorSetLayout = std::move(descriptorSetLayout),
           .vkPipeline = std::move(pipeline),
           .vkPipelineLayout = std::move(pipelineLayout),
           .vkRenderPass = std::move(renderPass)
        };
    }

    vk::UniqueRenderPass VulkanGraphicsPipelineFactory::createRenderPass(wlo::rendering::RenderPath) {
        std::array<vk::AttachmentDescription, 2> attachmentDescriptions;
        //color attachments (i.e the color image that's going to the screen)
        attachmentDescriptions[0] = vk::AttachmentDescription(vk::AttachmentDescriptionFlags(),
                                                              m_swapchain.getSwapSurfaceFormat(),
                                                              vk::SampleCountFlagBits::e1,
                                                              vk::AttachmentLoadOp::eClear,
                                                              vk::AttachmentStoreOp::eStore,
                                                              vk::AttachmentLoadOp::eDontCare,
                                                              vk::AttachmentStoreOp::eDontCare,
                                                              vk::ImageLayout::eUndefined,
                                                              vk::ImageLayout::ePresentSrcKHR);
        //depth attachment (the depth buffer used for coloring)
        attachmentDescriptions[1] = vk::AttachmentDescription(vk::AttachmentDescriptionFlags(),
                                                              vk::Format::eD16Unorm,
                                                              vk::SampleCountFlagBits::e1,
                                                              vk::AttachmentLoadOp::eClear,
                                                              vk::AttachmentStoreOp::eDontCare,
                                                              vk::AttachmentLoadOp::eDontCare,
                                                              vk::AttachmentStoreOp::eDontCare,
                                                              vk::ImageLayout::eUndefined,
                                                              vk::ImageLayout::eDepthStencilAttachmentOptimal);

        vk::AttachmentReference colorReference(0, vk::ImageLayout::eColorAttachmentOptimal);
        vk::AttachmentReference depthReference(1, vk::ImageLayout::eDepthStencilAttachmentOptimal);
        vk::SubpassDescription  subpass(
                vk::SubpassDescriptionFlags(), vk::PipelineBindPoint::eGraphics, {}, colorReference, {}, &depthReference);

        return  m_root.Device().createRenderPassUnique(
                vk::RenderPassCreateInfo(vk::RenderPassCreateFlags(), attachmentDescriptions, subpass));
    }

    void VulkanGraphicsPipelineFactory::rebuildGraphicsPipeline(GraphicsPipeline & pipeline) {
       WILO_CORE_INFO("Rebuilding pipeline with id {0}, to fit the new screen size of x: {1}, y: {2}",pipeline.id,
                                    m_swapchain.getSwapSurfaceExtent().width,m_swapchain.getSwapSurfaceExtent().height);
        pipeline.vkRenderPass = std::move(createRenderPass(pipeline.originPath));
        pipeline.vkPipeline = std::move(createPipeline(pipeline.originPath,pipeline.vkRenderPass,pipeline.vkPipelineLayout));
    }



    vk::UniquePipeline
    VulkanGraphicsPipelineFactory::createPipeline(wlo::rendering::RenderPath path,
                                                  vk::UniqueRenderPass & renderPass,
                                                  vk::UniquePipelineLayout & pipelineLayout) {
        VulkanShader & vertexShader = m_shaderCompiler.fetchShader(path.vertexShaderPath);
        VulkanShader & fragmentShader = m_shaderCompiler.fetchShader(path.fragmentShaderPath);


        DataLayout vertexLayout = vertexShader.vertexInputLayout();
        vk::VertexInputBindingDescription vertexInputBindingDescription(0, vertexLayout.memSize());
        std::vector<vk::VertexInputAttributeDescription> vertexInputAttributeDescriptions = tovkVertexAttributes(vertexLayout);

        vk::PipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo(
                vk::PipelineVertexInputStateCreateFlags(),  // flags
                vertexInputBindingDescription,              // vertexBindingDescriptions
                vertexInputAttributeDescriptions            // vertexAttributeDescriptions
        );

        vk::PipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo(
                vk::PipelineInputAssemblyStateCreateFlags(), vk::PrimitiveTopology::eTriangleList);

        vk::PipelineViewportStateCreateInfo pipelineViewportStateCreateInfo(
                vk::PipelineViewportStateCreateFlags(), 1, nullptr, 1, nullptr);

        vk::PipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo(
                vk::PipelineRasterizationStateCreateFlags(),  // flags
                false,                                        // depthClampEnable
                false,                                        // rasterizerDiscardEnable
                vk::PolygonMode::eFill,                       // polygonMode
                vk::CullModeFlagBits::eFront,                  // cullMode
                vk::FrontFace::eClockwise,                    // frontFace
                false,                                        // depthBiasEnable
                0.0f,                                         // depthBiasConstantFactor
                0.0f,                                         // depthBiasClamp
                0.0f,                                         // depthBiasSlopeFactor
                1.0f                                          // lineWidth
        );

        vk::PipelineMultisampleStateCreateInfo pipelineMultisampleStateCreateInfo(
                vk::PipelineMultisampleStateCreateFlags(),  // flags
                vk::SampleCountFlagBits::e1                 // rasterizationSamples
                // other values can be default
        );

        vk::StencilOpState stencilOpState(
                vk::StencilOp::eKeep, vk::StencilOp::eKeep, vk::StencilOp::eKeep, vk::CompareOp::eAlways);
        vk::PipelineDepthStencilStateCreateInfo pipelineDepthStencilStateCreateInfo(
                vk::PipelineDepthStencilStateCreateFlags(),  // flags
                true,                                        // depthTestEnable
                true,                                        // depthWriteEnable
                vk::CompareOp::eLessOrEqual,                 // depthCompareOp
                false,                                       // depthBoundTestEnable
                false,                                       // stencilTestEnable
                stencilOpState,                              // front
                stencilOpState                               // back
        );

        vk::ColorComponentFlags colorComponentFlags(vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
                                                    vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA);
        vk::PipelineColorBlendAttachmentState pipelineColorBlendAttachmentState(
                false,                   // blendEnable
                vk::BlendFactor::eZero,  // srcColorBlendFactor
                vk::BlendFactor::eZero,  // dstColorBlendFactor
                vk::BlendOp::eAdd,       // colorBlendOp
                vk::BlendFactor::eZero,  // srcAlphaBlendFactor
                vk::BlendFactor::eZero,  // dstAlphaBlendFactor
                vk::BlendOp::eAdd,       // alphaBlendOp
                colorComponentFlags      // colorWriteMask
        );
        vk::PipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo(
                vk::PipelineColorBlendStateCreateFlags(),  // flags
                false,                                     // logicOpEnable
                vk::LogicOp::eNoOp,                        // logicOp
                pipelineColorBlendAttachmentState,         // attachments
                {{1.0f, 1.0f, 1.0f, 1.0f}}             // blendConstants
        );

        std::array<vk::DynamicState, 2> dynamicStates = {vk::DynamicState::eViewport, vk::DynamicState::eScissor};
        vk::PipelineDynamicStateCreateInfo pipelineDynamicStateCreateInfo(vk::PipelineDynamicStateCreateFlags(),
                                                                          dynamicStates);
        std::vector<vk::PipelineShaderStageCreateInfo> shaderStageInfos = {
                vertexShader.pipelineStageInfo(),
                fragmentShader.pipelineStageInfo()
        };
        vk::GraphicsPipelineCreateInfo graphicsPipelineCreateInfo(
                vk::PipelineCreateFlags(),              // flags
                shaderStageInfos,
                &pipelineVertexInputStateCreateInfo,    // pVertexInputState
                &pipelineInputAssemblyStateCreateInfo,  // pInputAssemblyState
                nullptr,                                // pTessellationState
                &pipelineViewportStateCreateInfo,       // pViewportState
                &pipelineRasterizationStateCreateInfo,  // pRasterizationState
                &pipelineMultisampleStateCreateInfo,    // pMultisampleState
                &pipelineDepthStencilStateCreateInfo,   // pDepthStencilState
                &pipelineColorBlendStateCreateInfo,     // pColorBlendState
                &pipelineDynamicStateCreateInfo,        // pDynamicState
                pipelineLayout.get(),                   // layout
                renderPass.get()                       //renderPass
        );

        vk::Result result;
        vk::UniquePipeline pipeline;
        std::tie(result, pipeline) =
                m_root.Device().createGraphicsPipelineUnique(nullptr, graphicsPipelineCreateInfo).asTuple();

        WILO_CORE_INFO("CREATED GRAPHICS PIPLINE");
        return std::move(pipeline);
    }

    vk::UniqueDescriptorSetLayout VulkanGraphicsPipelineFactory::createDescriptorSetLayout(wlo::rendering::RenderPath) {
        vk::DescriptorSetLayoutBinding uniformBufferBinding(
                0, vk::DescriptorType::eUniformBuffer, 1, vk::ShaderStageFlagBits::eVertex);

        vk::DescriptorSetLayoutBinding textureBinding(
                1, vk::DescriptorType::eCombinedImageSampler, 1, vk::ShaderStageFlagBits::eFragment);

        std::array bindings = {uniformBufferBinding,textureBinding};
        return m_root.Device().createDescriptorSetLayoutUnique(
                                vk::DescriptorSetLayoutCreateInfo(vk::DescriptorSetLayoutCreateFlags(),
                                                                  bindings)
                                                                 );
    }


}
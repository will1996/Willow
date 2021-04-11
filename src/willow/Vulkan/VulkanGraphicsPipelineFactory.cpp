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


    std::vector<vk::VertexInputAttributeDescription> tovkVertexAttributes(const wlo::data::Type&  layout) {
            std::vector<vk::VertexInputAttributeDescription> desc;
            uint32_t bindingLocationCounter = 0;
            for(auto & [name,type,offset] : layout.getMembers()){
                        if(type.compatibleWith(wlo::data::Type::of<wlo::Vec2>()))
                            desc.push_back(vk::VertexInputAttributeDescription(bindingLocationCounter,0,vk::Format::eR32G32Sfloat,offset));
                        else if(type.compatibleWith(wlo::data::Type::of<wlo::Vec3>()))
                            desc.push_back(vk::VertexInputAttributeDescription(bindingLocationCounter,0,vk::Format::eR32G32B32Sfloat,offset));
                        else if(type.compatibleWith(wlo::data::Type::of<wlo::Vec4>()))
                            desc.push_back(vk::VertexInputAttributeDescription(bindingLocationCounter,0,vk::Format::eR32G32B32A32Sfloat,offset));
                        else
                            throw std::runtime_error("Vertex Input description for Data Layout element type FLOAT with count greater than four unsupported");
                        bindingLocationCounter++;
                }
            return desc;
   }

    vk::UniquePipelineLayout VulkanGraphicsPipelineFactory::createPipelineLayout(wlo::rendering::Material,
                                                                                 vk::UniqueDescriptorSetLayout &descriptorSetLayout,
                                                                                 wlo::data::Type pushLayout) {


        std::vector<vk::PushConstantRange> pushConstantRanges = {
                vk::PushConstantRange(vk::ShaderStageFlagBits::eVertex,0,pushLayout.footprint())
        };

        // create a PipelineLayout using that DescriptorSetLayout
       return m_root.Device().createPipelineLayoutUnique(
                vk::PipelineLayoutCreateInfo(vk::PipelineLayoutCreateFlags(), descriptorSetLayout.get(), pushConstantRanges));

    }


    wlo::data::Type makeAnonymousType(std::vector<wlo::data::Type> types) {
       std::vector<wlo::data::Type::Member > members(types.size());
       size_t i =0;
       for(auto & type : types)
           members[i++] = {"",type};
       return wlo::data::Type("",members);

   }

    wlo::wk::GraphicsPipeline
    wlo::wk::VulkanGraphicsPipelineFactory::buildGraphicsPipeline(wlo::rendering::Material mat,vk::UniqueRenderPass & renderPass) {

        VulkanShader vertexShader = m_shaderCompiler.ShaderFromBinary(vk::ShaderStageFlagBits::eVertex,mat.vertexShader);
        VulkanShader fragmentShader = m_shaderCompiler.ShaderFromBinary(vk::ShaderStageFlagBits::eFragment,mat.fragmentShader);

        wlo::data::Type vertexLayout = makeAnonymousType(vertexShader.vertexInputLayout());
        wlo::data::Type uniformLayout = makeAnonymousType(vertexShader.uniformBufferLayout());
        wlo::data::Type pushLayout = makeAnonymousType(vertexShader.pushConstantLayout());

        auto descriptorSetLayout = createDescriptorSetLayout(mat);
        auto pipelineLayout = createPipelineLayout(mat,descriptorSetLayout,pushLayout);
        auto pipeline = createPipeline(mat,renderPass,pipelineLayout);
        return GraphicsPipeline{
           .id = mat.id,
           .material = mat,
           .vertexLayout = vertexLayout,
           .uniformBufferLayout = uniformLayout,
           .pushConstantLayout = pushLayout,
           .vkPipelineLayout = std::move(pipelineLayout),
           .vkPipeline = std::move(pipeline),
           .vkDescriptorSetLayout = std::move(descriptorSetLayout)
        };
    }





    vk::UniquePipeline
    VulkanGraphicsPipelineFactory::createPipeline(wlo::rendering::Material mat,
                                                  vk::UniqueRenderPass &  renderPass,
                                                  vk::UniquePipelineLayout & pipelineLayout) {
        VulkanShader & vertexShader = m_shaderCompiler.fetchShader(mat.vertexShader);
        VulkanShader & fragmentShader = m_shaderCompiler.fetchShader(mat.fragmentShader);


        wlo::data::Type vertexLayout = makeAnonymousType(vertexShader.vertexInputLayout());
        vk::VertexInputBindingDescription vertexInputBindingDescription(0, vertexLayout.footprint());
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
                vk::CullModeFlagBits::eBack,                  // cullMode
                vk::FrontFace::eCounterClockwise,                    // frontFace
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
                true,                   // blendEnable
                vk::BlendFactor::eSrcAlpha,  // srcColorBlendFactor
                vk::BlendFactor::eOneMinusSrcAlpha,  // dstColorBlendFactor
                vk::BlendOp::eAdd,       // colorBlendOp
                vk::BlendFactor::eOne,  // srcAlphaBlendFactor
                vk::BlendFactor::eOne,  // dstAlphaBlendFactor
                vk::BlendOp::eAdd,       // alphaBlendOp
                colorComponentFlags
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

        WILO_CORE_INFO("CREATED GRAPHICS PIPLINE FOR MATERIAL ID {0} ",mat.id);
        return pipeline;
    }

    vk::UniqueDescriptorSetLayout VulkanGraphicsPipelineFactory::createDescriptorSetLayout(wlo::rendering::Material) {
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

#include "willow/rendering/Renderer.hpp"
#include "willow/root/wilo_dev_core.hpp"
#include "willow/Vulkan/VulkanRoot.hpp"
#include "willow/Vulkan/VulkanSwapchain.hpp"
#include"willow/Vulkan/VulkanShaderCompiler.hpp"
#include <glfw/glfw3.h>
namespace wlo::rendering{
    class VulkanImplementation {
    private:
        struct FrameClass {
            vector<RenderPath> paths;
            map<wlo::ID_type, vector<Frame::Draw>> DrawsPerPath;
            vector<std::pair<DataLayout, size_t> > vertexInputDescriptions;
        };

        struct MappedBuffer {
            vk::UniqueBuffer buffer;
            vk::DeviceMemory memory;
            byte* writePoint;
        };
        
        struct GraphicsPipeline {
            vk::UniquePipelineLayout layout;
            vk::UniquePipeline pipeline;
            vk::UniqueRenderPass renderPass;
        };

        const glm::mat4x4 m_clipMatrix = {//vulkan specific clip matrix (used for all camera transforms)
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, -1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 0.5f, 0.0f,
                0.0f, 0.0f, 0.5f, 1.0f
        };
        wk::VulkanRoot m_root;
        wk::VulkanSwapchain m_swapchain;
        wk::VulkanShaderCompiler m_shaderFactory;
        vk::UniqueCommandPool m_commandPool;
        vk::UniqueDescriptorPool  m_descriptorPool;
        vk::Queue m_graphicsQueue;
        vk::Queue m_presentQueue;


        std::map<wlo::ID_type, std::vector<vk::UniqueFramebuffer>> m_FrameBuffers;
        std::map<wlo::ID_type, vk::UniquePipeline > m_GraphicsPipelines;
        std::map<wlo::ID_type, vk::UniqueRenderPass > m_RenderPasses;
        std::map<wlo::ID_type, vk::UniqueDescriptorSet > m_DescriptorSets;

        std::map<wlo::ID_type, vk::UniquePipelineLayout> m_PipelineLayouts;
        
        std::unordered_map<DataLayout, MappedBuffer> m_VertexBuffers;
        std::unordered_map<wlo::ID_type, MappedBuffer> m_UniformBuffers;


        


    public: 
        VulkanImplementation(std::initializer_list<Renderer::Features> features, SharedPointer<Window> window, bool enableDebugging = true) :
            m_root(features, window.get(), enableDebugging),
            m_swapchain(m_root, window),
            m_shaderFactory(m_root)
            //m_memoryManager(m_vulkanRoot)
            //m_graphicsPipelineFactory(m_vulkanRoot)
        {
            //create command pool
            auto [graphicsIndex, presentIndex] = m_root.QueueFamilyIndices();
            vk::CommandPoolCreateInfo poolCreateInfo(vk::CommandPoolCreateFlags(), graphicsIndex);
            m_commandPool = m_root.Device().createCommandPoolUnique(poolCreateInfo);
            //create descriptor pool
            vk::DescriptorPoolSize   poolSize(vk::DescriptorType::eUniformBuffer, 10);
            m_descriptorPool = m_root.Device().createDescriptorPoolUnique(
                vk::DescriptorPoolCreateInfo(vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet, 10, poolSize));

            auto [GraphicsQueueIndex, PresentQueueIndex] = m_root.QueueFamilyIndices();
            m_graphicsQueue = m_root.Device().getQueue(GraphicsQueueIndex, 0);
            m_presentQueue = m_root.Device().getQueue(PresentQueueIndex, 0);
            //create memory manager
        }
        void PrepareFrameClass(const Frame& example) {
            FrameClass frameClass = buildClassFromExample(example);
            buildVertexBuffers(frameClass);
            buildAttachments(frameClass);
            buildUniformBuffers(frameClass);
            buildGraphicsPipelines(frameClass);
        }

        void buildVertexBuffers( const FrameClass & frameClass) {
            for (auto & [layout, count] : frameClass.vertexInputDescriptions) 
                m_VertexBuffers[layout] = allocateMappedBuffer(layout, count, vk::BufferUsageFlagBits::eVertexBuffer );
        }

        vector<std::pair<DataLayout, size_t>> vertexDescription(const vector<Draw>& draws) {
            std::unordered_map<DataLayout, size_t> layoutCounts;
            for (auto& draw : draws) {
                DataView vertexView = draw.geo.vertices;
                layoutCounts[vertexView.layout] += vertexView.count;
            }
            vector<std::pair<DataLayout, size_t>> vertexdesc;
            for (auto& [layout, count] : layoutCounts)
                vertexdesc.push_back(std::make_pair(layout, count));
            
            return vertexdesc;
        }

        FrameClass buildClassFromExample(const Frame& example) {
            FrameClass frameClass;
            set<ID_type> pathIds;
            for (const auto& draw : example.getDraws()) {
                if (!pathIds.contains(draw.path.id)) {
                    pathIds.insert(draw.path.id);
                    frameClass.paths.emplace_back(draw.path);
                }
                frameClass.DrawsPerPath[draw.path.id].emplace_back(draw);
            }
            frameClass.vertexInputDescriptions = vertexDescription(example.getDraws());
            return frameClass;
        }


        MappedBuffer allocateMappedBuffer(const DataLayout& layout ,size_t count , vk::BufferUsageFlagBits usage) {
            MappedBuffer buff;
            vk::BufferCreateInfo buffInfo(vk::BufferCreateFlags(), count * layout.memSize(), usage);
            buff.buffer = m_root.Device().createBufferUnique(buffInfo);
            vk::MemoryRequirements memoryRequirements = m_root.Device().getBufferMemoryRequirements(buff.buffer.get());

            uint32_t     vertexMemoryTypeIndex = m_root.findMemoryType(memoryRequirements,
                vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);

            vk::MemoryAllocateInfo allocInfo(memoryRequirements.size,vertexMemoryTypeIndex);
            buff.memory = m_root.Device().allocateMemory(allocInfo);

            buff.writePoint = (byte*) m_root.Device().mapMemory(buff.memory,0,memoryRequirements.size);

            memset(buff.writePoint, 0, memoryRequirements.size);
            
            m_root.Device().bindBufferMemory(*buff.buffer, buff.memory, 0);
            return buff;
        }
        
        void buildAttachments(FrameClass frameClass) {
            for (RenderPath & path : frameClass.paths) {
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

                m_RenderPasses[path.id] = m_root.Device().createRenderPassUnique(
                    vk::RenderPassCreateInfo(vk::RenderPassCreateFlags(), attachmentDescriptions, subpass));

                std::array<vk::ImageView, 2> attachments;
                attachments[1] = m_swapchain.getDepthImageView();

                m_FrameBuffers[path.id].reserve(m_swapchain.getSwapSurfaceViews().size());
                for (auto const& view : m_swapchain.getSwapSurfaceViews())
                {
                    attachments[0] = view;
                    m_FrameBuffers[path.id].push_back(m_root.Device().createFramebufferUnique(vk::FramebufferCreateInfo(vk::FramebufferCreateFlags(),
                        m_RenderPasses[path.id].get(),
                        attachments,
                        m_swapchain.getSwapSurfaceExtent().width,
                        m_swapchain.getSwapSurfaceExtent().height,
                        1)));
                }
            }
        }



        void buildGraphicsPipelines(const FrameClass& fc) {
            for (auto path : fc.paths) {
                auto vertexShaderModule = m_shaderFactory.makeModuleFromBinary(vk::ShaderStageFlagBits::eVertex, path.vertexShaderPath);
                auto fragmentShaderModule = m_shaderFactory.makeModuleFromBinary(vk::ShaderStageFlagBits::eFragment, path.fragmentShaderPath);

                std::array<vk::PipelineShaderStageCreateInfo, 2> pipelineShaderStageCreateInfos = {
                        vk::PipelineShaderStageCreateInfo(
                                vk::PipelineShaderStageCreateFlags(), vk::ShaderStageFlagBits::eVertex, vertexShaderModule.get(), "main"),
                        vk::PipelineShaderStageCreateInfo(
                                vk::PipelineShaderStageCreateFlags(), vk::ShaderStageFlagBits::eFragment, fragmentShaderModule.get(), "main")
                };

                vk::VertexInputBindingDescription                  vertexInputBindingDescription(0, sizeof(glm::vec4)*2);

                WILO_CORE_WARNING("VERTEX INPUT CURRENTLY FIXED TO DEFAULT FORMAT, WILL FAIL if vertices are anything other than a pair of glm::vec4");
                std::array<vk::VertexInputAttributeDescription, 2> vertexInputAttributeDescriptions = {
                        vk::VertexInputAttributeDescription(0, 0, vk::Format::eR32G32B32A32Sfloat, 0),
                        vk::VertexInputAttributeDescription(1, 0, vk::Format::eR32G32B32A32Sfloat, 16)
                };


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
                    { { 1.0f, 1.0f, 1.0f, 1.0f } }             // blendConstants
                );

                std::array<vk::DynamicState, 2>    dynamicStates = { vk::DynamicState::eViewport, vk::DynamicState::eScissor };
                vk::PipelineDynamicStateCreateInfo pipelineDynamicStateCreateInfo(vk::PipelineDynamicStateCreateFlags(),
                    dynamicStates);

                          vk::GraphicsPipelineCreateInfo graphicsPipelineCreateInfo(
                              vk::PipelineCreateFlags(),              // flags
                              pipelineShaderStageCreateInfos,         // stages
                              &pipelineVertexInputStateCreateInfo,    // pVertexInputState
                              &pipelineInputAssemblyStateCreateInfo,  // pInputAssemblyState
                              nullptr,                                // pTessellationState
                              &pipelineViewportStateCreateInfo,       // pViewportState
                              &pipelineRasterizationStateCreateInfo,  // pRasterizationState
                              &pipelineMultisampleStateCreateInfo,    // pMultisampleState
                              &pipelineDepthStencilStateCreateInfo,   // pDepthStencilState
                              &pipelineColorBlendStateCreateInfo,     // pColorBlendState
                              &pipelineDynamicStateCreateInfo,        // pDynamicState
                              m_PipelineLayouts[path.id].get(),                   // layout
                              m_RenderPasses[path.id].get()                        // renderPass
                          );

                          vk::Result         result;
                          vk::UniquePipeline pipeline;
                          std::tie(result, pipeline) =
                              m_root.Device().createGraphicsPipelineUnique(nullptr, graphicsPipelineCreateInfo).asTuple();
                          m_GraphicsPipelines[path.id] = std::move(pipeline);
            }
        }

        void buildUniformBuffers(const FrameClass& fc)
        {
            for (auto path : fc.paths) {

                m_UniformBuffers[path.id].buffer = m_root.Device().createBufferUnique(
                    vk::BufferCreateInfo{ vk::BufferCreateFlags(),sizeof(glm::mat4x4),vk::BufferUsageFlagBits::eUniformBuffer }
                );

                vk::MemoryRequirements uniformBufferMemRequirements = m_root.Device().getBufferMemoryRequirements(m_UniformBuffers[path.id].buffer.get());
                uint32_t memoryTypeIndex = m_root.findMemoryType(uniformBufferMemRequirements, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);

                m_UniformBuffers[path.id].memory = m_root.Device().allocateMemory(vk::MemoryAllocateInfo{ uniformBufferMemRequirements.size, memoryTypeIndex });
                m_UniformBuffers[path.id].writePoint = (static_cast<byte*>(m_root.Device().mapMemory(m_UniformBuffers[path.id].memory, 0, uniformBufferMemRequirements.size)));

                glm::mat4x4 view = glm::lookAt(glm::vec3(-5.0f, 3.0f, -10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
                glm::mat4x4 projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
                glm::mat4x4 mvpc = m_clipMatrix * projection * view;

                memcpy(m_UniformBuffers[path.id].writePoint, &mvpc, sizeof(mvpc));
                m_root.Device().bindBufferMemory(m_UniformBuffers[path.id].buffer.get(), m_UniformBuffers[path.id].memory, 0);

                vk::DescriptorSetLayoutBinding descriptorSetLayoutBinding(
                    0, vk::DescriptorType::eUniformBuffer, 1, vk::ShaderStageFlagBits::eVertex);

                std::vector<vk::DescriptorSetLayout> descriptorSetLayouts = {
                    m_root.Device().createDescriptorSetLayout(
                        vk::DescriptorSetLayoutCreateInfo(vk::DescriptorSetLayoutCreateFlags(), descriptorSetLayoutBinding)),
                };

                std::vector<vk::PushConstantRange> pushConstantRanges = {
                    vk::PushConstantRange(vk::ShaderStageFlagBits::eVertex,0,sizeof(glm::mat4x4))
                };

                // create a PipelineLayout using that DescriptorSetLayout
                m_PipelineLayouts[path.id] = m_root.Device().createPipelineLayoutUnique(
                    vk::PipelineLayoutCreateInfo(vk::PipelineLayoutCreateFlags(), descriptorSetLayouts, pushConstantRanges));



                // allocate a descriptor set
                m_DescriptorSets[path.id] = std::move(
                    m_root.Device().allocateDescriptorSetsUnique(vk::DescriptorSetAllocateInfo(*m_descriptorPool, descriptorSetLayouts)).front());

                //copy data from the uniform buffer into the descriptor so that it is accessible by the shaders
                vk::DescriptorBufferInfo descriptorBufferInfo(m_UniformBuffers[path.id].buffer.get(), 0, sizeof(glm::mat4x4));
                m_root.Device().updateDescriptorSets(
                    vk::WriteDescriptorSet(*m_DescriptorSets[path.id], 0, 0, vk::DescriptorType::eUniformBuffer, {}, descriptorBufferInfo),
                    {});

            }

        }

        void updateUnifromBuffer(wlo::ID_type pathID,DataLayout layout, size_t offset, const byte*  data, size_t count) {
            memcpy(&m_UniformBuffers[pathID].writePoint[offset], data, layout.memSize());
        }

        void updateVertexBuffer(DataLayout layout, size_t offset, const byte * data, size_t count) {
            memcpy(&m_VertexBuffers[layout].writePoint[offset], data, count*layout.memSize());
        }

        void submit(const Frame& frame) {
            const uint64_t FenceTimeout = 100000000;
            // Get the index of the next available swapchain image:
            vk::UniqueSemaphore       imageAcquiredSemaphore = m_root.Device().createSemaphoreUnique(vk::SemaphoreCreateInfo());
            vk::ResultValue<uint32_t> currentBuffer = m_root.Device().acquireNextImageKHR(
                m_swapchain.get(), FenceTimeout, imageAcquiredSemaphore.get(), nullptr);
            assert(currentBuffer.result == vk::Result::eSuccess);
            assert(currentBuffer.value < m_swapchain.getSwapSurfaceViews().size());

            vk::UniqueCommandBuffer commandBuffer = std::move(m_root.Device().allocateCommandBuffersUnique(vk::CommandBufferAllocateInfo(
                m_commandPool.get(), vk::CommandBufferLevel::ePrimary, 1))
                .front());
            commandBuffer->begin(vk::CommandBufferBeginInfo(vk::CommandBufferUsageFlags()));

            std::array<vk::ClearValue, 2> clearValues;
            clearValues[0].color = vk::ClearColorValue(std::array<float, 4>({ { 0.7f, 0.1f, 0.2f, 0.2f } }));
            clearValues[1].depthStencil = vk::ClearDepthStencilValue(1.0f, 0);
            ID_type  pathId = frame.getDraws()[0].path.id;

            vk::RenderPassBeginInfo renderPassBeginInfo(m_RenderPasses[pathId].get(),
                m_FrameBuffers[pathId][currentBuffer.value].get(),
                vk::Rect2D(vk::Offset2D(0, 0), m_swapchain.getSwapSurfaceExtent()),
                clearValues);
            commandBuffer->beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);

            commandBuffer->bindPipeline(vk::PipelineBindPoint::eGraphics, m_GraphicsPipelines[pathId].get());
            commandBuffer->setViewport(0,
                vk::Viewport(0.0f,
                    0.0f,
                    static_cast<float>(m_swapchain.getSwapSurfaceExtent().width),
                    static_cast<float>(m_swapchain.getSwapSurfaceExtent().height),
                    0.0f,
                    1.0f));

            commandBuffer->setScissor(0, vk::Rect2D(vk::Offset2D(0, 0), m_swapchain.getSwapSurfaceExtent()));

            size_t drawOffset = 0;
            for (const Draw& draw : frame.getDraws()) {
                 
                commandBuffer->bindDescriptorSets(
                    vk::PipelineBindPoint::eGraphics, m_PipelineLayouts[pathId].get(), 0, *m_DescriptorSets[pathId], nullptr);

                updateVertexBuffer(draw.geo.vertices.layout, drawOffset, draw.geo.vertices.source, draw.geo.vertices.count);
                
                commandBuffer->bindVertexBuffers(0, *m_VertexBuffers[draw.geo.vertices.layout].buffer, { drawOffset });
                commandBuffer->pushConstants(m_PipelineLayouts[pathId].get(), vk::ShaderStageFlagBits::eVertex, 0,sizeof(glm::mat4x4),&draw.geo.ModelMatrix);
                commandBuffer->draw(draw.geo.vertices.count, 1, 0, 0);

                drawOffset += draw.geo.vertices.memSize;
            }
            commandBuffer->endRenderPass();
            commandBuffer->end();

            vk::UniqueFence drawFence = m_root.Device().createFenceUnique(vk::FenceCreateInfo());

            vk::PipelineStageFlags waitDestinationStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput);
            vk::SubmitInfo         submitInfo(*imageAcquiredSemaphore, waitDestinationStageMask, *commandBuffer);
            m_graphicsQueue.submit(submitInfo, drawFence.get());

            while (vk::Result::eTimeout == m_root.Device().waitForFences(drawFence.get(), VK_TRUE, FenceTimeout))
                ;
            std::array<vk::SwapchainKHR, 1> swapchains = { m_swapchain.get() };
            vk::PresentInfoKHR presentInfo({}, swapchains, currentBuffer.value);
            vk::Result result =
                m_presentQueue.presentKHR(presentInfo);
            switch (result)
            {
            case vk::Result::eSuccess: break;
            case vk::Result::eSuboptimalKHR:
                std::cout << "vk::Queue::presentKHR returned vk::Result::eSuboptimalKHR !\n";
                break;
            default: assert(false);  // an unexpected result is returned !
            }

        }



        ~VulkanImplementation() {
        }

        


    };
//RENDERER


   Renderer::Renderer(SharedPointer<Window> window,std::initializer_list<Features> Features){
       wlo::logr::initalize();
       pImpl=wlo::CreateUniquePointer<VulkanImplementation>(Features,window);
   }


    Renderer::~Renderer() {
        WILO_CORE_INFO("Renderer deconstructed");
    }



    void Renderer::PrepareFrameClass(const Frame & example) {
        pImpl->PrepareFrameClass(example);
    }

    void Renderer::Submit(const Frame & frame) {
        pImpl->submit(frame);
    }

    auto Renderer::shareRoots() {
       return nullptr;
    }


}


#include "willow/rendering/Renderer.hpp"
#include "willow/root/wilo_dev_core.hpp"
#include "willow/Vulkan/VulkanRoot.hpp"
#include "willow/Vulkan/VulkanSwapchain.hpp"
#include"willow/Vulkan/VulkanShaderCompiler.hpp"
#include <glfw/glfw3.h>
namespace wlo::rendering{
    class VulkanImplementation{
    public:
#ifndef NDEBUG
        VulkanImplementation(std::initializer_list<Renderer::Features>,SharedPointer<Window>,bool enableDebugging = true);
#else
        VulkanImplementation(std::initializer_list<Renderer::Features>,SharedPointer<Window>, bool enableDebugging = false);
#endif
        void setup(wlo::rendering::RenderPath);
        void submit(const Frame &);
        ~VulkanImplementation();
    private:
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
        std::map<wlo::ID_type,vk::UniqueBuffer > m_UniformBuffers;
        std::map<wlo::ID_type,vk::UniqueDeviceMemory > m_UniformBufferMemory;
        std::map<wlo::ID_type,vk::UniqueBuffer > m_VertexBuffers;
        std::map<wlo::ID_type,vk::UniqueDeviceMemory > m_VertexBufferMemory;
        std::map<wlo::ID_type ,std::vector<vk::UniqueFramebuffer>> m_FrameBuffers;
        std::map<wlo::ID_type,vk::UniquePipeline > m_GraphicsPipelines;
        std::map<wlo::ID_type,vk::UniqueRenderPass > m_RenderPasses;
        std::map<wlo::ID_type ,vk::UniqueDescriptorSet> m_DescriptorSets;
        std::map<wlo::ID_type ,vk::UniquePipelineLayout> m_PipelineLayouts;

        void updateUnifromBuffer(wlo::ID_type,const glm::mat4x4 & newValue);
        void writeVertexBuffer(wlo::ID_type,const DataView& view);
        void buildGraphicsPipeline(const RenderPath &);

    };


    VulkanImplementation::~VulkanImplementation() {
    }



    VulkanImplementation::VulkanImplementation(std::initializer_list<Renderer::Features> features,SharedPointer<Window> window, bool enableDebugging):
            m_root(features, window.get(), enableDebugging),
            m_swapchain(m_root, window),
            m_shaderFactory(m_root)
    //m_memoryManager(m_vulkanRoot)
    //m_graphicsPipelineFactory(m_vulkanRoot)
    {
        //create command pool
        auto [graphicsIndex, presentIndex] = m_root.QueueFamilyIndices();
        vk::CommandPoolCreateInfo poolCreateInfo( vk::CommandPoolCreateFlags(), graphicsIndex );
        m_commandPool = m_root.Device().createCommandPoolUnique(poolCreateInfo);
        //create descriptor pool
        vk::DescriptorPoolSize   poolSize( vk::DescriptorType::eUniformBuffer, 1 );
        m_descriptorPool = m_root.Device().createDescriptorPoolUnique(
                vk::DescriptorPoolCreateInfo( vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet, 1, poolSize ) );

        auto[GraphicsQueueIndex,PresentQueueIndex] = m_root.QueueFamilyIndices();
        m_graphicsQueue = m_root.Device().getQueue(GraphicsQueueIndex,0);
        m_presentQueue = m_root.Device().getQueue(PresentQueueIndex,0);

    }

    void VulkanImplementation::setup(wlo::rendering::RenderPath path) {
        m_UniformBuffers[path.id] = m_root.Device().createBufferUnique(
                vk::BufferCreateInfo{vk::BufferCreateFlags(),sizeof(glm::mat4x4),vk::BufferUsageFlagBits::eUniformBuffer}
                );

        vk::MemoryRequirements uniformBufferMemRequirements = m_root.Device().getBufferMemoryRequirements(m_UniformBuffers[path.id].get());
        uint32_t memoryTypeIndex = m_root.findMemoryType(uniformBufferMemRequirements, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
        m_UniformBufferMemory[path.id] =  m_root.Device().allocateMemoryUnique(vk::MemoryAllocateInfo{uniformBufferMemRequirements.size, memoryTypeIndex});
        uint8_t * pData =
                static_cast<uint8_t *>( m_root.Device().mapMemory(m_UniformBufferMemory[path.id].get(), 0, uniformBufferMemRequirements.size ) );

        glm::mat4x4 model = glm::mat4x4( 1.0f );
        glm::mat4x4 view =
                glm::lookAt( glm::vec3( -5.0f, 3.0f, -10.0f ), glm::vec3( 0.0f, 0.0f, 0.0f ), glm::vec3( 0.0f, -1.0f, 0.0f ) );
        glm::mat4x4 projection = glm::perspective( glm::radians( 45.0f ), 1.0f, 0.1f, 100.0f );
        glm::mat4x4 clip       = glm::mat4x4( 1.0f,
                                              0.0f,
                                              0.0f,
                                              0.0f,
                                              0.0f,
                                              -1.0f,
                                              0.0f,
                                              0.0f,
                                              0.0f,
                                              0.0f,
                                              0.5f,
                                              0.0f,
                                              0.0f,
                                              0.0f,
                                              0.5f,
                                              1.0f );  // vulkan clip space has inverted y and half z !
        glm::mat4x4 mvpc       = clip * projection * view * model;
        memcpy( pData, &mvpc, sizeof( mvpc ) );
        m_root.Device().unmapMemory( m_UniformBufferMemory[path.id].get() );
        m_root.Device().bindBufferMemory( m_UniformBuffers[path.id].get(), m_UniformBufferMemory[path.id].get(), 0 );

        vk::DescriptorSetLayoutBinding descriptorSetLayoutBinding(
                0, vk::DescriptorType::eUniformBuffer, 1, vk::ShaderStageFlagBits::eVertex );
        vk::UniqueDescriptorSetLayout descriptorSetLayout = m_root.Device().createDescriptorSetLayoutUnique(
                vk::DescriptorSetLayoutCreateInfo( vk::DescriptorSetLayoutCreateFlags(), descriptorSetLayoutBinding ) );

        // create a PipelineLayout using that DescriptorSetLayout
        m_PipelineLayouts[path.id] = m_root.Device().createPipelineLayoutUnique(
                vk::PipelineLayoutCreateInfo( vk::PipelineLayoutCreateFlags(), *descriptorSetLayout ) );
        // allocate a descriptor set
        m_DescriptorSets[path.id] = std::move(
                m_root.Device().allocateDescriptorSetsUnique( vk::DescriptorSetAllocateInfo( *m_descriptorPool, *descriptorSetLayout ) )
                        .front() );
        //copy data from the uniform buffer into the descriptor so that it is accessible by the shaders
        vk::DescriptorBufferInfo descriptorBufferInfo( m_UniformBuffers[path.id].get(), 0, sizeof( glm::mat4x4 ) );
        m_root.Device().updateDescriptorSets(
                vk::WriteDescriptorSet( m_DescriptorSets[path.id].get(), 0, 0, vk::DescriptorType::eUniformBuffer, {}, descriptorBufferInfo ),
                {} );

        std::array<vk::AttachmentDescription, 2> attachmentDescriptions;
        //color attachments (i.e the color image that's going to the screen)
        attachmentDescriptions[0] = vk::AttachmentDescription( vk::AttachmentDescriptionFlags(),
                                                               m_swapchain.getSwapSurfaceFormat(),
                                                               vk::SampleCountFlagBits::e1,
                                                               vk::AttachmentLoadOp::eClear,
                                                               vk::AttachmentStoreOp::eStore,
                                                               vk::AttachmentLoadOp::eDontCare,
                                                               vk::AttachmentStoreOp::eDontCare,
                                                               vk::ImageLayout::eUndefined,
                                                               vk::ImageLayout::ePresentSrcKHR );
       //depth attachment (the depth buffer used for coloring)
        attachmentDescriptions[1] = vk::AttachmentDescription( vk::AttachmentDescriptionFlags(),
                                                               vk::Format::eD16Unorm,
                                                               vk::SampleCountFlagBits::e1,
                                                               vk::AttachmentLoadOp::eClear,
                                                               vk::AttachmentStoreOp::eDontCare,
                                                               vk::AttachmentLoadOp::eDontCare,
                                                               vk::AttachmentStoreOp::eDontCare,
                                                               vk::ImageLayout::eUndefined,
                                                               vk::ImageLayout::eDepthStencilAttachmentOptimal );

        vk::AttachmentReference colorReference( 0, vk::ImageLayout::eColorAttachmentOptimal );
        vk::AttachmentReference depthReference( 1, vk::ImageLayout::eDepthStencilAttachmentOptimal );
        vk::SubpassDescription  subpass(
                vk::SubpassDescriptionFlags(), vk::PipelineBindPoint::eGraphics, {}, colorReference, {}, &depthReference );

        m_RenderPasses[path.id] = m_root.Device().createRenderPassUnique(
                vk::RenderPassCreateInfo( vk::RenderPassCreateFlags(), attachmentDescriptions, subpass ) );
        auto vertexShaderModule  = m_shaderFactory.makeModuleFromBinary(vk::ShaderStageFlagBits::eVertex,path.vertexShaderPath);
        auto fragmentShaderModule =  m_shaderFactory.makeModuleFromBinary(vk::ShaderStageFlagBits::eFragment,path.fragmentShaderPath);

        std::array<vk::ImageView, 2> attachments;
        attachments[1] = m_swapchain.getDepthImageView();

        m_FrameBuffers[path.id].reserve( m_swapchain.getSwapSurfaceViews().size() );
        for ( auto const & view : m_swapchain.getSwapSurfaceViews())
        {
            attachments[0] = view;
            m_FrameBuffers[path.id].push_back( m_root.Device().createFramebufferUnique( vk::FramebufferCreateInfo( vk::FramebufferCreateFlags(),
                                                                                                m_RenderPasses[path.id].get(),
                                                                                                attachments,
                                                                                                m_swapchain.getSwapSurfaceExtent().width,
                                                                                                m_swapchain.getSwapSurfaceExtent().height,
                                                                                                1 ) ) );
        }
        auto [vertexLayout,count] = path.vertexInputDescription;
        // create a vertex buffer for some vertex and color data
        m_VertexBuffers[path.id] = m_root.Device().createBufferUnique( vk::BufferCreateInfo(
                vk::BufferCreateFlags(), vertexLayout.memSize()*count, vk::BufferUsageFlagBits::eVertexBuffer ) );

        // allocate device memory for that buffer
        vk::MemoryRequirements memoryRequirements = m_root.Device().getBufferMemoryRequirements( m_VertexBuffers[path.id].get() );
        uint32_t               vertexMemoryTypeIndex  = m_root.findMemoryType(memoryRequirements,vk::MemoryPropertyFlagBits::eHostVisible|vk::MemoryPropertyFlagBits::eHostCoherent);
        m_VertexBufferMemory[path.id] =
                m_root.Device().allocateMemoryUnique( vk::MemoryAllocateInfo( memoryRequirements.size,vertexMemoryTypeIndex ) );

        byte* write = (byte*) m_root.Device().mapMemory(m_VertexBufferMemory[path.id].get(),0,vertexLayout.memSize()*count);


        memset(write,0,count*vertexLayout.memSize());
        m_root.Device().unmapMemory(m_VertexBufferMemory[path.id].get());

        // and bind the device memory to the vertex buffer
        m_root.Device().bindBufferMemory( m_VertexBuffers[path.id].get(), m_VertexBufferMemory[path.id].get(), 0 );

        std::array<vk::PipelineShaderStageCreateInfo, 2> pipelineShaderStageCreateInfos = {
                vk::PipelineShaderStageCreateInfo(
                        vk::PipelineShaderStageCreateFlags(), vk::ShaderStageFlagBits::eVertex, vertexShaderModule.get(), "main" ),
                vk::PipelineShaderStageCreateInfo(
                        vk::PipelineShaderStageCreateFlags(), vk::ShaderStageFlagBits::eFragment, fragmentShaderModule.get(), "main" )
        };

        vk::VertexInputBindingDescription                  vertexInputBindingDescription( 0, vertexLayout.memSize() );

        WILO_CORE_WARNING("VERTEX INPUT CURRENTLY FIXED TO DEFAULT FORMAT, WILL FAIL if vertices are anything other than a pair of glm::vec4");
        std::array<vk::VertexInputAttributeDescription, 2> vertexInputAttributeDescriptions = {
                vk::VertexInputAttributeDescription( 0, 0, vk::Format::eR32G32B32A32Sfloat, 0 ),
                vk::VertexInputAttributeDescription( 1, 0, vk::Format::eR32G32B32A32Sfloat, 16 )
        };


        vk::PipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo(
                vk::PipelineVertexInputStateCreateFlags(),  // flags
                vertexInputBindingDescription,              // vertexBindingDescriptions
                vertexInputAttributeDescriptions            // vertexAttributeDescriptions
        );

        vk::PipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo(
                vk::PipelineInputAssemblyStateCreateFlags(), vk::PrimitiveTopology::eTriangleList );

        vk::PipelineViewportStateCreateInfo pipelineViewportStateCreateInfo(
                vk::PipelineViewportStateCreateFlags(), 1, nullptr, 1, nullptr );

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
                vk::StencilOp::eKeep, vk::StencilOp::eKeep, vk::StencilOp::eKeep, vk::CompareOp::eAlways );
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

        vk::ColorComponentFlags colorComponentFlags( vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
                                                     vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA );
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
        vk::PipelineDynamicStateCreateInfo pipelineDynamicStateCreateInfo( vk::PipelineDynamicStateCreateFlags(),
                                                                           dynamicStates );

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
        std::tie( result, pipeline ) =
                m_root.Device().createGraphicsPipelineUnique( nullptr, graphicsPipelineCreateInfo ).asTuple();
        m_GraphicsPipelines[path.id] = std::move(pipeline);
       }


    void VulkanImplementation::updateUnifromBuffer(wlo::ID_type, const glm::mat4x4 &newValue) {

    }

    void VulkanImplementation::buildGraphicsPipeline(const RenderPath &) {

    }

    void VulkanImplementation::submit(const Frame & frame) {
        const uint64_t FenceTimeout = 100000000;
        // Get the index of the next available swapchain image:
        vk::UniqueSemaphore       imageAcquiredSemaphore = m_root.Device().createSemaphoreUnique( vk::SemaphoreCreateInfo() );
        vk::ResultValue<uint32_t> currentBuffer          = m_root.Device().acquireNextImageKHR(
                m_swapchain.get(), FenceTimeout, imageAcquiredSemaphore.get(), nullptr );
        assert( currentBuffer.result == vk::Result::eSuccess );
        assert( currentBuffer.value < m_swapchain.getSwapSurfaceViews().size() );

        vk::UniqueCommandBuffer commandBuffer = std::move( m_root.Device().allocateCommandBuffersUnique( vk::CommandBufferAllocateInfo(
                                                                           m_commandPool.get(), vk::CommandBufferLevel::ePrimary, 1 ) )
                                                                   .front() );
        commandBuffer->begin( vk::CommandBufferBeginInfo( vk::CommandBufferUsageFlags() ) );

        std::array<vk::ClearValue, 2> clearValues;
        clearValues[0].color        = vk::ClearColorValue( std::array<float, 4>( { { 0.7f, 0.1f, 0.2f, 0.2f } } ) );
        clearValues[1].depthStencil = vk::ClearDepthStencilValue( 1.0f, 0 );

        for( const Draw & draw : frame.getDraws()){
            ID_type  pathId = draw.path.id;
            //writeVertexBuffer(pathId,draw.geo.verticies[-1]);
            vk::RenderPassBeginInfo renderPassBeginInfo( m_RenderPasses[pathId].get(),
                                                         m_FrameBuffers[pathId][currentBuffer.value].get(),
                                                         vk::Rect2D( vk::Offset2D( 0, 0 ), m_swapchain.getSwapSurfaceExtent()),
                                                         clearValues );
            commandBuffer->beginRenderPass( renderPassBeginInfo, vk::SubpassContents::eInline );
            commandBuffer->bindPipeline( vk::PipelineBindPoint::eGraphics, m_GraphicsPipelines[pathId].get() );
            commandBuffer->bindDescriptorSets(
                    vk::PipelineBindPoint::eGraphics, m_PipelineLayouts[pathId].get(), 0, m_DescriptorSets[pathId].get(), nullptr );

            commandBuffer->bindVertexBuffers( 0,*m_VertexBuffers[pathId] , { 0 } );
            commandBuffer->setViewport( 0,
                                        vk::Viewport( 0.0f,
                                                      0.0f,
                                                      static_cast<float>( m_swapchain.getSwapSurfaceExtent().width ),
                                                      static_cast<float>( m_swapchain.getSwapSurfaceExtent().height ),
                                                      0.0f,
                                                      1.0f ) );
            commandBuffer->setScissor( 0, vk::Rect2D( vk::Offset2D( 0, 0 ), m_swapchain.getSwapSurfaceExtent()) );

            commandBuffer->draw( 12 * 3, 1, 0, 0 );
            commandBuffer->endRenderPass();
        }
        commandBuffer->end();

        vk::UniqueFence drawFence = m_root.Device().createFenceUnique( vk::FenceCreateInfo() );

        vk::PipelineStageFlags waitDestinationStageMask( vk::PipelineStageFlagBits::eColorAttachmentOutput );
        vk::SubmitInfo         submitInfo( *imageAcquiredSemaphore, waitDestinationStageMask, *commandBuffer );
        m_graphicsQueue.submit( submitInfo, drawFence.get() );

        while ( vk::Result::eTimeout == m_root.Device().waitForFences( drawFence.get(), VK_TRUE, FenceTimeout ) )
            ;
        std::array<vk::SwapchainKHR,1> swapchains = {m_swapchain.get()};
        vk::PresentInfoKHR presentInfo({},swapchains,currentBuffer.value);
        vk::Result result =
                m_presentQueue.presentKHR(presentInfo);
        switch ( result )
        {
            case vk::Result::eSuccess: break;
            case vk::Result::eSuboptimalKHR:
                std::cout << "vk::Queue::presentKHR returned vk::Result::eSuboptimalKHR !\n";
                break;
            default: assert( false );  // an unexpected result is returned !
        }

    }

    void VulkanImplementation::writeVertexBuffer(wlo::ID_type pathId, const DataView &view) {
        byte* write = (byte*) m_root.Device().mapMemory(m_VertexBufferMemory[pathId].get(),0,view.memSize);
        memcpy(write,view.source,view.memSize);
        m_root.Device().unmapMemory(m_VertexBufferMemory[pathId].get());
    }

//RENDERER


   Renderer::Renderer(SharedPointer<Window> window, std::initializer_list<RenderPath> paths,std::initializer_list<Features> Features){
       wlo::logr::initalize();
       pImpl=wlo::CreateUniquePointer<VulkanImplementation>(Features,window);
       for(auto & path : paths)
           Setup(path);
   }


    Renderer::~Renderer() {
        WILO_CORE_INFO("Renderer deconstructed");
    }


   void Renderer::Setup(const RenderPath & path) {
        pImpl->setup(path);
    }

    void Renderer::Prepare(const Frame &) {

    }

    void Renderer::Submit(const Frame & frame) {
        pImpl->submit(frame);
    }

    auto Renderer::shareRoots() {
       return nullptr;
    }


}


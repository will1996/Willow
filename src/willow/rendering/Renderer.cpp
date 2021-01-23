#include "willow/rendering/Renderer.hpp"
#include "willow/root/wilo_dev_core.hpp"
#include "willow/Vulkan/VulkanRoot.hpp"
#include "willow/Vulkan/VulkanSwapchain.hpp"
#include"willow/Vulkan/VulkanShaderCompiler.hpp"
#include "willow/Vulkan/VulkanGraphicsPipelineFactory.hpp"
#include "willow/Vulkan/VulkanMemoryManager.hpp"
#include "willow/Vulkan/VulkanCommandInterface.hpp"
#include"willow/Vulkan/VulkanTextureFactory.hpp"
#include <glfw/glfw3.h>
namespace wlo::rendering{
    class VulkanImplementation :public wlo::MessageSystem::Observer{
    private:
        struct FrameClass {
            vector<RenderPath> paths;
            map<wlo::ID_type, vector<Frame::Draw>> DrawsPerPath;
            vector<std::pair<DataLayout, size_t> > vertexInputDescriptions;
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
        wk::VulkanGraphicsPipelineFactory m_pipelineFactory;
        wk::VulkanCommandInterface m_commandInterface;
        wk::VulkanMemoryManger m_memoryManager;
        wk::VulkanTextureFactory m_textureFactory;
        vk::UniqueDescriptorPool  m_descriptorPool;



        vk::Queue m_presentQueue;
        std::map<wlo::ID_type, wk::GraphicsPipeline > m_GraphicsPipelines;
        std::map<wlo::ID_type, std::vector<vk::UniqueFramebuffer>> m_FrameBuffers;
        std::map<wlo::ID_type, vk::UniqueDescriptorSet > m_DescriptorSets;
        std::map<wlo::ID_type, vk::UniqueSampler > m_textureSamplers;
        std::map<wlo::ID_type, std::string> m_textures;

        std::unordered_map<wlo::ID_type,wk::MappedBuffer> m_UniformBuffers;

        std::unordered_map<DataLayout, wk::MappedBuffer> m_VertexBuffers;

        wk::MappedBuffer m_IndexBuffer;
        glm::vec4 m_clearColor;





    public:
        glm::vec4 nextClearColor;
        VulkanImplementation(std::initializer_list<Renderer::Features> features, SharedPointer<Window> window, bool enableDebugging = true) :
            m_root(features, window.get(), enableDebugging),
            m_swapchain(m_root, window),
            m_shaderFactory(m_root),
            m_pipelineFactory(m_root,m_shaderFactory,m_swapchain),
            m_commandInterface(m_root),
            m_memoryManager(m_root,m_commandInterface),
            m_textureFactory(m_root,m_memoryManager)

            //m_memoryManager(m_vulkanRoot)
            //m_graphicsPipelineFactory(m_vulkanRoot)
        {
            window->permit<wlo::WindowResized,VulkanImplementation,&VulkanImplementation::resizeDrawSurface>(this);
            //create descriptor pool
            std::array   poolSizes = {vk::DescriptorPoolSize(vk::DescriptorType::eUniformBuffer, 1),vk::DescriptorPoolSize(vk::DescriptorType::eCombinedImageSampler, 1)};
            m_descriptorPool = m_root.Device().createDescriptorPoolUnique(
                vk::DescriptorPoolCreateInfo(vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet, 10, poolSizes));
            auto [_, PresentQueueIndex] = m_root.QueueFamilyIndices();
            m_presentQueue = m_root.Device().getQueue(PresentQueueIndex, 0);

        }

        void resizeDrawSurface(const wlo::WindowResized &msg){
            cout<<"RESIZING"<<endl;
            m_root.Device().waitIdle();
            m_swapchain.resize();
            for(auto &[_,pipeline] : m_GraphicsPipelines) {
                //m_pipelineFactory.rebuildGraphicsPipeline(pipeline);
                RenderPath & pathref = pipeline.originPath;
                m_FrameBuffers[pathref.id].clear();
                buildFrameBuffers(pathref);
            }

        }
        void PrepareFrameClass(const Frame& example) {
            FrameClass frameClass = buildClassFromExample(example);
            buildTextures(frameClass);
            buildGraphicsPipelines(frameClass);
            buildVertexBuffers(frameClass);
            buildIndexBuffers(frameClass);
        }

        void buildVertexBuffers( const FrameClass & frameClass) {
            for (auto & [layout, count] : frameClass.vertexInputDescriptions) 
                m_VertexBuffers[layout] = m_memoryManager.allocateMappedBuffer(layout, count, vk::BufferUsageFlagBits::eVertexBuffer );
        }
        void buildIndexBuffers(FrameClass & frameClass){
            size_t indexCount = 0;
            for(auto & path: frameClass.paths) {
                for (auto &draw: frameClass.DrawsPerPath[path.id])
                    indexCount += draw.geo.indices.count;
            }
            m_IndexBuffer = m_memoryManager.allocateMappedBuffer(Layout<Index>(), indexCount, vk::BufferUsageFlagBits::eIndexBuffer);
        }

        void buildTextures(FrameClass & frameClass){
            for(auto & path :frameClass.paths)
                for(auto & draw : frameClass.DrawsPerPath[path.id])
                    for(std::string & texturePath: draw.geo.texturesPaths)
                        if(!texturePath.empty()&&!m_textureFactory.textureCreated(texturePath)) {
                            m_textureFactory.createTexture2D(texturePath);
                            m_textures[path.id] = texturePath;
                        }
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





        void buildGraphicsPipelines(FrameClass& fc) {
            for (auto path : fc.paths) {
                    m_GraphicsPipelines[path.id] = m_pipelineFactory.buildGraphicsPipeline(path);

                    for(auto draw: fc.DrawsPerPath[path.id])
                        assert(draw.geo.vertices.layout==m_GraphicsPipelines[path.id].vertexLayout);

                    buildUniformBuffers(m_GraphicsPipelines[path.id]);
                    buildFrameBuffers(path);

            }
        }

        void buildFrameBuffers(const wlo::rendering::RenderPath & path){
            std::array<vk::ImageView, 2> attachments;
            attachments[1] = m_swapchain.getDepthImageView();

            m_FrameBuffers[path.id].reserve(m_swapchain.getSwapSurfaceViews().size());
            for (auto const& view : m_swapchain.getSwapSurfaceViews())
            {
                attachments[0] = view;
                m_FrameBuffers[path.id].push_back(m_root.Device().createFramebufferUnique(vk::FramebufferCreateInfo(vk::FramebufferCreateFlags(),
                                                                                                                    m_GraphicsPipelines[path.id].vkRenderPass.get(),
                                                                                                                    attachments,
                                                                                                                    m_swapchain.getSwapSurfaceExtent().width,
                                                                                                                    m_swapchain.getSwapSurfaceExtent().height,
                                                                                                                    1)));
            }

        }

        void buildUniformBuffers(const wk::GraphicsPipeline & pipeline )
        {

            m_UniformBuffers[pipeline.id].buffer = m_root.Device().createBufferUnique(
                    vk::BufferCreateInfo{ vk::BufferCreateFlags(),sizeof(glm::mat4x4),vk::BufferUsageFlagBits::eUniformBuffer }
                );

                vk::MemoryRequirements uniformBufferMemRequirements = m_root.Device().getBufferMemoryRequirements(m_UniformBuffers[pipeline.id].buffer.get());
                uint32_t memoryTypeIndex = m_root.findMemoryType(uniformBufferMemRequirements, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);

                m_UniformBuffers[pipeline.id].memory = std::move(m_root.Device().allocateMemoryUnique(vk::MemoryAllocateInfo{ uniformBufferMemRequirements.size, memoryTypeIndex }));
                m_UniformBuffers[pipeline.id].writePoint = (static_cast<byte*>(m_root.Device().mapMemory(m_UniformBuffers[pipeline.id].memory.get(), 0, uniformBufferMemRequirements.size)));

                glm::mat4x4 view = glm::lookAt(glm::vec3(-5.0f, 3.0f, -10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
                glm::mat4x4 projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
                glm::mat4x4 mvpc = m_clipMatrix * projection * view;

                memcpy(m_UniformBuffers[pipeline.id].writePoint, &mvpc, sizeof(mvpc));
                m_root.Device().bindBufferMemory(m_UniformBuffers[pipeline.id].buffer.get(), m_UniformBuffers[pipeline.id].memory.get(), 0);

                // allocate a descriptor set
                std::array<vk::DescriptorSetLayout,1> vkDescriptorSetLayouts{pipeline.vkDescriptorSetLayout.get()};
                m_DescriptorSets[pipeline.id] = std::move(
                    m_root.Device().allocateDescriptorSetsUnique(vk::DescriptorSetAllocateInfo(*m_descriptorPool, vkDescriptorSetLayouts)).front());


                //copy data from the uniform buffer into the descriptor so that it is accessible by the shaders
                vk::DescriptorBufferInfo descriptorBufferInfo(m_UniformBuffers[pipeline.id].buffer.get(), 0, sizeof(glm::mat4x4));
                m_root.Device().updateDescriptorSets(
                    vk::WriteDescriptorSet(*m_DescriptorSets[pipeline.id], 0, 0, vk::DescriptorType::eUniformBuffer, {}, descriptorBufferInfo),
                    {});

                m_textureSamplers[pipeline.id] =
                    m_root.Device().createSamplerUnique( vk::SamplerCreateInfo( vk::SamplerCreateFlags(),
                                                                        vk::Filter::eNearest,
                                                                        vk::Filter::eNearest,
                                                                        vk::SamplerMipmapMode::eNearest,
                                                                        vk::SamplerAddressMode::eClampToEdge,
                                                                        vk::SamplerAddressMode::eClampToEdge,
                                                                        vk::SamplerAddressMode::eClampToEdge,
                                                                        0.0f,
                                                                        false,
                                                                        1.0f,
                                                                        false,
                                                                        vk::CompareOp::eNever,
                                                                        0.0f,
                                                                        0.0f,
                                                                        vk::BorderColor::eFloatOpaqueWhite ) );
           auto & texture =  m_textureFactory.fetchTexture(m_textures[pipeline.id]);
           vk::DescriptorImageInfo samplerWrite(m_textureSamplers[pipeline.id].get(),texture.view.get());
           samplerWrite.imageLayout = texture.layout;
           m_root.Device().updateDescriptorSets(
                   vk::WriteDescriptorSet(m_DescriptorSets[pipeline.id].get(),1,0,vk::DescriptorType::eCombinedImageSampler,samplerWrite,{}),
                   {}
                   );

        }

        void updateUnifromBuffer(wlo::ID_type pathID,const glm::mat4x4 &uniformView) {

            glm::mat4x4 mvpc = m_clipMatrix * uniformView;
            memcpy(m_UniformBuffers[pathID].writePoint,&mvpc, sizeof(mvpc));
        }

        void updateVertexBuffer(DataView vertView, size_t offset) {
            memcpy(&m_VertexBuffers[vertView.layout].writePoint[offset], vertView.source, vertView.memSize);
        }

        void updateIndexBuffer(DataView indexView,size_t offset){
            memcpy(&m_IndexBuffer.writePoint[offset], indexView.source, indexView.memSize);
        }


        void submit(const Frame& frame) {
            // Get the index of the next available swapchain image:
            vk::UniqueSemaphore       imageAcquiredSemaphore = m_root.Device().createSemaphoreUnique(vk::SemaphoreCreateInfo());
            vk::ResultValue<uint32_t> currentBuffer = m_root.Device().acquireNextImageKHR(
                m_swapchain.get(), m_commandInterface.FenceTimeout, imageAcquiredSemaphore.get(), nullptr);
            assert(currentBuffer.result == vk::Result::eSuccess);
            assert(currentBuffer.value < m_swapchain.getSwapSurfaceViews().size());
            vk::UniqueCommandBuffer commandBuffer = m_commandInterface.newCommandBuffer();
            commandBuffer->begin(vk::CommandBufferBeginInfo(vk::CommandBufferUsageFlags()));

            std::array<vk::ClearValue, 2> clearValues;
            m_clearColor = nextClearColor;
            clearValues[0].color = vk::ClearColorValue(std::array<float, 4>({ { m_clearColor.x, m_clearColor.y, m_clearColor.z, m_clearColor.w } }));
            clearValues[1].depthStencil = vk::ClearDepthStencilValue(1.0f, 0);
            ID_type  pathId = frame.getDraws()[0].path.id;

            vk::RenderPassBeginInfo renderPassBeginInfo(m_GraphicsPipelines[pathId].vkRenderPass.get(),
                m_FrameBuffers[pathId][currentBuffer.value].get(),
                vk::Rect2D(vk::Offset2D(0, 0), m_swapchain.getSwapSurfaceExtent()),
                clearValues);
            commandBuffer->beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);

            commandBuffer->bindPipeline(vk::PipelineBindPoint::eGraphics, m_GraphicsPipelines[pathId].vkPipeline.get());
            commandBuffer->setViewport(0,
                vk::Viewport(0.0f,
                    0.0f,
                    static_cast<float>(m_swapchain.getSwapSurfaceExtent().width),
                    static_cast<float>(m_swapchain.getSwapSurfaceExtent().height),
                    0.0f,
                    1.0f));

            commandBuffer->setScissor(0, vk::Rect2D(vk::Offset2D(0, 0), m_swapchain.getSwapSurfaceExtent()));

            size_t vertexOffset = 0;
            size_t indexOffset = 0;
            for (const Draw& draw : frame.getDraws()) {
                 
                commandBuffer->bindDescriptorSets(
                    vk::PipelineBindPoint::eGraphics, m_GraphicsPipelines[pathId].vkPipelineLayout.get(), 0, *m_DescriptorSets[pathId], nullptr);

                updateVertexBuffer(draw.geo.vertices, vertexOffset);
                updateIndexBuffer(draw.geo.indices,indexOffset);
                updateUnifromBuffer(draw.path.id,draw.path.camera->getTransform());

                commandBuffer->bindVertexBuffers(0, *m_VertexBuffers[draw.geo.vertices.layout].buffer, { vertexOffset });
                commandBuffer->bindIndexBuffer(*m_IndexBuffer.buffer,indexOffset,vk::IndexType::eUint32);
                commandBuffer->pushConstants(m_GraphicsPipelines[pathId].vkPipelineLayout.get(), vk::ShaderStageFlagBits::eVertex, 0,sizeof(glm::mat4x4),&draw.geo.ModelMatrix);
                commandBuffer->drawIndexed(draw.geo.indices.count,1,0,0,0);

                vertexOffset += draw.geo.vertices.memSize;
                indexOffset+= draw.geo.indices.memSize;
            }
            commandBuffer->endRenderPass();
            commandBuffer->end();

            vk::UniqueFence drawFence = m_root.Device().createFenceUnique(vk::FenceCreateInfo());

            vk::PipelineStageFlags waitDestinationStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput);
            vk::SubmitInfo         submitInfo(*imageAcquiredSemaphore, waitDestinationStageMask, *commandBuffer);
            m_commandInterface.submitGraphicsCommands(submitInfo,drawFence);

            while (vk::Result::eTimeout == m_root.Device().waitForFences(drawFence.get(), VK_TRUE, m_commandInterface.FenceTimeout))
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
            default: WILO_CORE_ERROR("INVALID RENDER SURFACE");
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

    void Renderer::setClearColor(wlo::Color color) {
       pImpl->nextClearColor = color.color;
    }


}


#include "willow/root/Root.hpp"
#include "willow/rendering/Renderer.hpp"
#include "willow/rendering/PerspectiveCamera3D.hpp"
#include "willow/Vulkan/VulkanRoot.hpp"
#include "willow/Vulkan/VulkanSwapchain.hpp"
#include"willow/Vulkan/VulkanShaderCompiler.hpp"
#include "willow/Vulkan/VulkanGraphicsPipelineFactory.hpp" 
#include "willow/Vulkan/VulkanCommandInterface.hpp"
#include"willow/Vulkan/VulkanTextureFactory.hpp"
#include "willow/Vulkan/VulkanSetup.hpp"
#include <GLFW/glfw3.h>
namespace wlo::rendering{


    struct Draw{
        View vertices;
        View indices;
        wlo::Mat4 modelMatrix;
        const Material& material;
    };


    class VulkanImplementation :public wlo::MessageSystem::Observer{
    public:
        Renderer::Statistics statistics;
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
        wk::VulkanGraphicsPipelineFactory m_pipelineFactory;
        wk::VulkanCommandInterface m_commandInterface;
        wk::VulkanMemoryManger m_memoryManager;
        wk::VulkanTextureFactory m_textureFactory;
        vk::UniqueDescriptorPool  m_descriptorPool;

        vk::Queue m_presentQueue;
        std::unordered_map<wlo::ID_type, wk::GraphicsPipeline > m_GraphicsPipelines;
        std::map<wlo::ID_type, vk::UniqueDescriptorSet > m_DescriptorSets;
        std::map<wlo::ID_type, vk::UniqueSampler > m_textureSamplers;

        vk::UniqueRenderPass m_defaultRenderPass;
        std::vector<vk::UniqueFramebuffer > m_frameBuffers;

        std::unordered_map<wlo::ID_type,wk::MappedBuffer> m_UniformBuffers;

        std::unordered_map<wlo::data::Type, wk::MappedBuffer> m_VertexBuffers;

        wk::MappedBuffer m_IndexBuffer;
        glm::vec4 m_clearColor;
        size_t m_frameCount;

        void createDefaultRenderPass() {
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

            m_defaultRenderPass = m_root.Device().createRenderPassUnique(
                    vk::RenderPassCreateInfo(vk::RenderPassCreateFlags(),attachmentDescriptions,subpass)
                    );
        }

    public:
        GPUInfo getGPUInfo(){
            auto deviceProperties = m_root.PhysicalDevice().getMemoryProperties();
            size_t memoryHeapSum = 0;
            for (auto & memoryHeap: deviceProperties.memoryHeaps){
                cout<<memoryHeap.size<<endl;
                memoryHeapSum+=memoryHeap.size;
                }

            return GPUInfo {
                .totalMemoryUsage = m_memoryManager.getTotalAllocationSize(),
                .totalMemoryAvailable = memoryHeapSum
            };
        }
        glm::vec4 nextClearColor;
        VulkanImplementation(std::initializer_list<Renderer::Features> features, Window& window, bool enableDebugging = true) :
            m_root(features, window, false),
            m_swapchain(m_root, window),
            m_shaderFactory(m_root),
            m_pipelineFactory(m_root,m_shaderFactory,m_swapchain),
            m_commandInterface(m_root),
            m_memoryManager(m_root,m_commandInterface),
            m_textureFactory(m_root,m_memoryManager),
            m_frameCount(0)
        {
            window.permit<wlo::WindowResized,VulkanImplementation,&VulkanImplementation::resizeDrawSurface>(this);
            //create descriptor pool
            std::array   poolSizes = {
                    vk::DescriptorPoolSize(vk::DescriptorType::eUniformBuffer, 10),
                    vk::DescriptorPoolSize(vk::DescriptorType::eCombinedImageSampler, 10)};
            m_descriptorPool = m_root.Device().createDescriptorPoolUnique(
                vk::DescriptorPoolCreateInfo(vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet, 10, poolSizes));
            auto [_, PresentQueueIndex] = m_root.QueueFamilyIndices();
            m_presentQueue = m_root.Device().getQueue(PresentQueueIndex, 0);
            createDefaultRenderPass();
            buildFrameBuffers();
            }

        void resizeDrawSurface(const wlo::WindowResized &msg){
            m_root.Device().waitIdle();
            m_swapchain.resize();
            m_frameBuffers.clear();
            buildFrameBuffers();
        }

        void prepare(const SceneDescription & description)  {
                buildTextures(description);
                buildGraphicsPipelines(description);
                buildVertexBuffers(description);
                buildIndexBuffers(description);
        }

        void prepareMaterials(std::vector<Material> mats){

        }
        void allocateVertexBuffers(std::vector<std::pair<wlo::data::Type,size_t>> vertexCounts ){

        }


        void buildVertexBuffers( const SceneDescription & description) {
            for (auto & [layout, count] : description.vertexCounts)
                m_VertexBuffers[layout] = m_memoryManager.allocateMappedBuffer(layout, count, vk::BufferUsageFlagBits::eVertexBuffer );
        }

        void buildIndexBuffers(const SceneDescription& description){
            m_IndexBuffer = m_memoryManager.allocateMappedBuffer(wlo::data::typeOf<Index>(), description.totalIndexCount, vk::BufferUsageFlagBits::eIndexBuffer);
        }

        void buildTextures(const SceneDescription & desc){
                for(auto & material : desc.materials){
                        if(!m_textureFactory.textureCreated(material->texture.id)) {
                            m_textureFactory.bindHostTexture(material->texture);
                        }
                }
        }


        vector<std::pair<wlo::data::Type, size_t>> vertexDescription(const vector<Draw>& draws) {
            std::unordered_map<wlo::data::Type, size_t> layoutCounts;
            for (auto& draw : draws) {
                View vertexView = draw.vertices;
                layoutCounts[vertexView.layout] += vertexView.count;
            }
            vector<std::pair<wlo::data::Type, size_t>> vertexdesc;
            for (auto& [layout, count] : layoutCounts)
                vertexdesc.push_back(std::make_pair(layout, count));
            
            return vertexdesc;
        }


        void buildGraphicsPipelines(const SceneDescription& desc) {
            for (auto &  mat : desc.materials) {
                    m_GraphicsPipelines[mat->id] = m_pipelineFactory.buildGraphicsPipeline(*mat,m_defaultRenderPass);
                    buildUniformBuffers(m_GraphicsPipelines[mat->id]);
            }
        }

        void buildFrameBuffers(){
            std::array<vk::ImageView, 2> attachments;
            attachments[1] = m_swapchain.getDepthImageView();
            for (auto const& view : m_swapchain.getSwapSurfaceViews())
            {
                attachments[0] = view;
                m_frameBuffers.push_back(
                m_root.Device().createFramebufferUnique(vk::FramebufferCreateInfo(vk::FramebufferCreateFlags(),
                                                                                  m_defaultRenderPass.get(),
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

                vk::MemoryRequirements uniformBufferMemRequirements = m_root.Device().getBufferMemoryRequirements(
                        m_UniformBuffers[pipeline.id].buffer.get()
                                            );
                uint32_t memoryTypeIndex = m_root.findMemoryType(uniformBufferMemRequirements,
                                                                 vk::MemoryPropertyFlagBits::eHostVisible |
                                                                 vk::MemoryPropertyFlagBits::eHostCoherent);

                m_UniformBuffers[pipeline.id].memory = m_root.Device().allocateMemoryUnique(
                                vk::MemoryAllocateInfo{ uniformBufferMemRequirements.size, memoryTypeIndex });
                m_UniformBuffers[pipeline.id].writePoint = (
                        static_cast<byte*>(m_root.Device().mapMemory(
                                m_UniformBuffers[pipeline.id].memory.get(),
                                    0,
                                    uniformBufferMemRequirements.size)));

                wlo::Mat4 uniformInit(1);
                memcpy(m_UniformBuffers[pipeline.id].writePoint, &uniformInit, sizeof(uniformInit));
                m_root.Device().bindBufferMemory(m_UniformBuffers[pipeline.id].buffer.get(),
                                                 m_UniformBuffers[pipeline.id].memory.get(), 0);

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
           auto & texture =  m_textureFactory.fetchTexture(pipeline.texID);
           vk::DescriptorImageInfo samplerWrite(m_textureSamplers[pipeline.id].get(), texture.view.get());
           samplerWrite.imageLayout = texture.layout;
           m_root.Device().updateDescriptorSets(
                   vk::WriteDescriptorSet(m_DescriptorSets[pipeline.id].get(), 1, 0, vk::DescriptorType::eCombinedImageSampler, samplerWrite, {}),
                   {}
                   );


        }

        void updateUnifromBuffer(wlo::ID_type pathID,const glm::mat4x4 &uniformView) {
            glm::mat4x4 mvpc =  uniformView;
            memcpy(m_UniformBuffers[pathID].writePoint,&mvpc, sizeof(mvpc));
        }

        void updateVertexBuffer(View vertView, size_t offset) {
            memcpy(&m_VertexBuffers[vertView.layout].writePoint[offset], vertView.source, vertView.memSize);
        }

        void updateIndexBuffer(View indexView, size_t offset){
            memcpy(&m_IndexBuffer.writePoint[offset], indexView.source, indexView.memSize);
        }


        void submit(std::vector<Draw> draws,const glm::mat4 cameraTransform) {
            auto currentTime = std::chrono::high_resolution_clock::now();
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

            vk::RenderPassBeginInfo renderPassBeginInfo(m_defaultRenderPass.get(),
                m_frameBuffers[currentBuffer.value].get(),
                vk::Rect2D(vk::Offset2D(0, 0), m_swapchain.getSwapSurfaceExtent()),
                clearValues);
            commandBuffer->beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);

            size_t vertexOffset = 0;
            size_t indexOffset = 0;
            for (const Draw& draw : draws) {
            commandBuffer->bindPipeline(vk::PipelineBindPoint::eGraphics, m_GraphicsPipelines[draw.material.id].vkPipeline.get());
            commandBuffer->setViewport(0,
                vk::Viewport(0.0f,
                    0.0f,
                    static_cast<float>(m_swapchain.getSwapSurfaceExtent().width),
                    static_cast<float>(m_swapchain.getSwapSurfaceExtent().height),
                    0.0f,
                    1.0f));

            commandBuffer->setScissor(0, vk::Rect2D(vk::Offset2D(0, 0), m_swapchain.getSwapSurfaceExtent()));

                commandBuffer->bindDescriptorSets(
                    vk::PipelineBindPoint::eGraphics, m_GraphicsPipelines[draw.material.id].vkPipelineLayout.get(), 0, *m_DescriptorSets[draw.material.id], nullptr);

                updateVertexBuffer(draw.vertices, vertexOffset);
                updateIndexBuffer(draw.indices,indexOffset);
                updateUnifromBuffer(draw.material.id,cameraTransform);

                commandBuffer->bindVertexBuffers(0, *m_VertexBuffers[draw.vertices.layout].buffer, { vertexOffset });
                commandBuffer->bindIndexBuffer(*m_IndexBuffer.buffer,indexOffset,vk::IndexType::eUint32);
                commandBuffer->pushConstants(m_GraphicsPipelines[draw.material.id].vkPipelineLayout.get(), vk::ShaderStageFlagBits::eVertex, 0,sizeof(glm::mat4x4),&draw.modelMatrix);
                commandBuffer->drawIndexed(draw.indices.count,1,0,0,0);

                vertexOffset += draw.vertices.memSize;
                indexOffset+= draw.indices.memSize;
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
            }
            auto endTime = std::chrono::high_resolution_clock::now();
            m_frameCount++;

            statistics.frameRate = 1.0/std::chrono::duration<double,std::chrono::seconds::period>(endTime-currentTime).count();
            statistics.frameCount = m_frameCount;
        }



        ~VulkanImplementation() {
        }

        


    };
//RENDERER


   Renderer::Renderer(std::initializer_list<Features> Features) {
   }

    void Renderer::connect(Messenger * messenger) {
       auto pwind = dynamic_cast<Window *>(messenger);
       if (pwind!=nullptr) {
           pImpl = wlo::CreateUniquePointer<VulkanImplementation>(std::initializer_list<Features>(), *pwind);
       }
       else{
            auto pcore = dynamic_cast<Core *>(messenger);
            if(pcore!=nullptr){
                this->permit<RenderStart,Renderer,&Renderer::draw>(pcore);
            }




       }


    }

    Renderer::~Renderer() {
    }




    void Renderer::setClearColor(wlo::Vec4 color) {
       pImpl->nextClearColor = color;
    }


    const Renderer::Statistics & Renderer::getStats(){
        return pImpl->statistics;
    }


    void Renderer::preAllocateScene(SceneDescription description) {

       pImpl->prepare(description);
    }

    void Renderer::render(const Scene & scene,const wlo::Mat4 & cameraMatrix) {
        std::vector<Draw> draws; draws.reserve(scene.m_objects.size());
        for (auto& renderObj : scene.m_objects) {
            draws.push_back(Draw{ .vertices = renderObj.mesh.vertexView(),.indices = renderObj.mesh.indexView(),.modelMatrix = renderObj.transform, .material = renderObj.material });
        }
        pImpl->submit(draws,cameraMatrix);
    }

    void Renderer::draw(const RenderStart &) {
    }


}



#include "vulkan_context.hpp"
#include "VulkanSwapchain.hpp"
#include "vulkan_frame_buffer_array.hpp"
#include "vulkan_vertex_buffer.hpp"
#include "vulkan_index_buffer.hpp"

#include "willow/dev-utils/Vulkan/wilovk.hpp"

namespace wilo{
    class VulkanRenderTaskManager {
            public:
             VulkanRenderTaskManager(wilo::SharedPointer<VulkanRenderingContext>,
                                     wilo::SharedPointer<VulkanSwapchain>, 
                                     wilo::SharedPointer<VulkanFrameBufferArray>);
             void reclaim() ;
             void reset() ;
             void waitOnCurrentTasks() ;
             void SetRenderPass(const VulkanRenderPass&);
             //void BindVertexBuffer(::SharedPointer<::VertexBuffer> p_VertexBuffer) ;
             //void BindIndexBuffer(::SharedPointer<::IndexBuffer> p_IndexBuffer) ;
             //void CreateDrawTask(::SharedPointer<::GraphicsPipeline>);
             //void SubmitDrawTask()override;

            private:
                const size_t m_numFrameBuffers;
                size_t m_currentFrameIndex;
                wilo::SharedPointer<VulkanRenderingContext> p_context;
                wilo::SharedPointer<VulkanSwapchain> p_swapchain;
                wilo::SharedPointer<VulkanFrameBufferArray> p_frameBufferArray;

                wilo::SharedPointer<VulkanVertexBuffer> p_vertexBuffer;
                wilo::SharedPointer<VulkanIndexBuffer> p_indexBuffer;

                VkRenderPass m_renderPass;

                VkCommandPool m_vkCommandPool;

                std::vector<VkSemaphore> imageAvailableSemaphores;
                std::vector<VkSemaphore> renderFinishedSemaphores;
                std::vector<VkFence> commandBufferExecutingFences;
                std::vector<VkFence> imageInUseFences;

                std::vector<VkCommandBuffer> m_drawCommandBuffers;
                VkQueue graphicsQueue;
                VkQueue presentQueue;

                void createCommandPool();
                void allocateDrawCommandBuffers();
                void createSynchronizationPrimitives();

    };



}
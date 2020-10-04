#include "willow/rendering/Vulkan/vulkan_render_task_manager.hpp"
#include "willow/rendering/Vulkan/vulkan_graphics_pipeline.hpp"


namespace wilo{
    VulkanRenderTaskManager::VulkanRenderTaskManager(wilo::SharedPointer<VulkanRenderingContext> contextRef,
                                                     wilo::SharedPointer<VulkanSwapchain> swapchainRef,
                                                     wilo::SharedPointer<VulkanFrameBufferArray> frameBufferArrayRef)
    : 
    p_context(contextRef),p_swapchain(swapchainRef),p_frameBufferArray(frameBufferArrayRef),
    m_numFrameBuffers(frameBufferArrayRef->size()),
    m_currentFrameIndex(0)
    {   
        vk::QueueFamilyIndices queueIndices = vk::findQueueFamilies(p_context->getPhysicalDevice(),p_context->getSurface());
        vkGetDeviceQueue(p_context->getDevice(),queueIndices.graphicsFamily.value(),0,&graphicsQueue);
        vkGetDeviceQueue(p_context->getDevice(),queueIndices.presentFamily.value(),0,&presentQueue);

        createCommandPool();

        WILO_CORE_INFO("Vulkan Task manager created command pool!");

        allocateDrawCommandBuffers();

        createSynchronizationPrimitives();

    }

    void VulkanRenderTaskManager::waitOnCurrentTasks(){
        vkDeviceWaitIdle(p_context->getDevice());
    }

    void VulkanRenderTaskManager::createCommandPool(){
        vk::QueueFamilyIndices queueIndices  = vk::findQueueFamilies( p_context->getPhysicalDevice(),p_context->getSurface());
        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.queueFamilyIndex = queueIndices.graphicsFamily.value();
        poolInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT|VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        VkResult res = vkCreateCommandPool(p_context->getDevice(),&poolInfo,nullptr,&m_vkCommandPool);
        vk::checkResult(res,"create command pool");

    }
    void VulkanRenderTaskManager::allocateDrawCommandBuffers(){
        m_drawCommandBuffers.resize(m_numFrameBuffers);
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandBufferCount = (uint32_t) m_drawCommandBuffers.size();
        allocInfo.commandPool = m_vkCommandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

        VkResult res = vkAllocateCommandBuffers(p_context->getDevice(),&allocInfo,m_drawCommandBuffers.data());

        vk::checkResult(res,"allocate draw command buffers");

    }
    void VulkanRenderTaskManager::SetRenderPass(const VulkanRenderPass& renderPassRef){
        m_renderPass = renderPassRef.get();
    } 
   // void VulkanRenderTaskManager::CreateDrawTask(::SharedPointer<::GraphicsPipeline> pipelineRef){
   //     //make sure that the last call to the current command buffer has finished executing
   //         vkWaitForFences(p_context->getDevice(),1,&commandBufferExecutingFences[m_currentFrameIndex],VK_TRUE,UINT64_MAX);

   //     ::SharedPointer<VulkanGraphicsPipeline>  vkPipelineRef = std::static_pointer_cast<VulkanGraphicsPipeline>(pipelineRef);

   //     VkCommandBufferBeginInfo bufferBeginInfo{};
   //     bufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
   //     bufferBeginInfo.flags =0;
   //     bufferBeginInfo.pInheritanceInfo = nullptr;
   //     vkBeginCommandBuffer(m_drawCommandBuffers[m_currentFrameIndex],&bufferBeginInfo);

   //     VkRenderPassBeginInfo passBeginInfo{};
   //     passBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
   //     passBeginInfo.renderPass = m_renderPass;
   //     passBeginInfo.framebuffer = (*p_frameBufferArray)[m_currentFrameIndex];
   //     passBeginInfo.renderArea.extent = p_swapchain->getSwapSurfaceExtent();
   //     passBeginInfo.clearValueCount = 1;
   //     VkClearValue clearColor = {0.0,0.0,1.0,1.0};//make the clear color BLUE
   //     passBeginInfo.pClearValues = &clearColor;

   //     vkCmdBeginRenderPass(m_drawCommandBuffers[m_currentFrameIndex],&passBeginInfo,VK_SUBPASS_CONTENTS_INLINE);

   //     vkCmdBindPipeline(m_drawCommandBuffers[m_currentFrameIndex],VK_PIPELINE_BIND_POINT_GRAPHICS,vkPipelineRef->get());
   //     VkDeviceSize offsets[] = { 0 };
   //     VkBuffer buffers[] = { p_vertexBuffer->get() };
   //     vkCmdBindVertexBuffers(m_drawCommandBuffers[m_currentFrameIndex], 0, 1, buffers, offsets);

   //     vkCmdBindIndexBuffer(m_drawCommandBuffers[m_currentFrameIndex], p_indexBuffer->get(),0,VK_INDEX_TYPE_UINT32);

   //     vkCmdDrawIndexed(m_drawCommandBuffers[m_currentFrameIndex],static_cast<uint32_t>(p_indexBuffer->size()),1,0,0,0);

   //     vkCmdEndRenderPass(m_drawCommandBuffers[m_currentFrameIndex]);

   //     VkResult res = vkEndCommandBuffer(m_drawCommandBuffers[m_currentFrameIndex]);
   //     vk::checkResult(res,"end command buffer");
   // }
    void VulkanRenderTaskManager::createSynchronizationPrimitives(){
        VkSemaphoreCreateInfo semInfo{};
        semInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        renderFinishedSemaphores.resize(m_numFrameBuffers);
        imageAvailableSemaphores.resize(m_numFrameBuffers);
        commandBufferExecutingFences.resize(m_numFrameBuffers);
        imageInUseFences.resize(m_numFrameBuffers,VK_NULL_HANDLE);

        for(size_t i=0;i<m_numFrameBuffers;i++){
        VkResult res1 = vkCreateSemaphore(p_context->getDevice(),&semInfo,nullptr,&renderFinishedSemaphores[i]);
        VkResult res2 = vkCreateSemaphore(p_context->getDevice(),&semInfo,nullptr,&imageAvailableSemaphores[i]);
        VkResult res3 = vkCreateFence(p_context->getDevice(),&fenceInfo,nullptr,&commandBufferExecutingFences[i]);
//        VkResult res4 = vkCreateFence(p_context->getDevice(),&fenceInfo,nullptr,&imageInUseFences[i]);

        vk::checkResult(res1, "create render finished semaphore");
        vk::checkResult(res2, "create image available semaphore");
        vk::checkResult(res3, "create command buffer executed fence");
 //       vk::checkResult(res4, "create image used fence");
        }

    }

   //void VulkanRenderTaskManager::BindVertexBuffer(::SharedPointer<::VertexBuffer > p_buffer) {
   //      p_vertexBuffer = std::static_pointer_cast<VulkanVertexBuffer>(p_buffer);
   //}
   //void VulkanRenderTaskManager::BindIndexBuffer(::SharedPointer<::IndexBuffer> p_IndexBuffer)
   //{
   //      p_indexBuffer = std::static_pointer_cast<VulkanIndexBuffer>(p_IndexBuffer);
   //}

   // void VulkanRenderTaskManager::SubmitDrawTask(){

   //     uint32_t imageIndex;
   //     vkAcquireNextImageKHR(p_context->getDevice(),p_swapchain->get(),UINT64_MAX,imageAvailableSemaphores[m_currentFrameIndex],VK_NULL_HANDLE,&imageIndex);
   //     WILO_ASSERT(imageIndex==m_currentFrameIndex);

   //     if(imageInUseFences[imageIndex]!=VK_NULL_HANDLE){
   //         vkWaitForFences(p_context->getDevice(),1,&imageInUseFences[imageIndex],VK_TRUE,UINT64_MAX);
   //     }
   //     imageInUseFences[imageIndex] = commandBufferExecutingFences[m_currentFrameIndex];

   //     VkSubmitInfo submitInfo{ };

   //     submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

   //     submitInfo.waitSemaphoreCount = 1;
   //     submitInfo.pWaitSemaphores = &imageAvailableSemaphores[m_currentFrameIndex];
   //     
   //     VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
   //     submitInfo.pWaitDstStageMask = waitStages;

   //     submitInfo.commandBufferCount = 1;
   //     submitInfo.pCommandBuffers = &m_drawCommandBuffers[imageIndex];

   //     submitInfo.signalSemaphoreCount = 1;
   //     submitInfo.pSignalSemaphores = &renderFinishedSemaphores[imageIndex];

   //     vkResetFences(p_context->getDevice(),1,&commandBufferExecutingFences[m_currentFrameIndex]);

   //     VkResult res1 = vkQueueSubmit(graphicsQueue,1,&submitInfo,commandBufferExecutingFences[m_currentFrameIndex]);
   //     vk::checkResult(res1,"submit draw command queue");

   //     VkPresentInfoKHR presentInfo{};

   //     presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
   //     presentInfo.waitSemaphoreCount = 1;
   //     presentInfo.pWaitSemaphores = &renderFinishedSemaphores[m_currentFrameIndex];
   //     VkSwapchainKHR swapchains[] = {p_swapchain->get()};
   //     presentInfo.pSwapchains = swapchains;

   //     presentInfo.swapchainCount = 1;

   //     presentInfo.pImageIndices = &imageIndex;

   //     presentInfo.pResults = nullptr;

   //     vkQueuePresentKHR(presentQueue,&presentInfo);

   //     m_currentFrameIndex+=1;
   //     m_currentFrameIndex%=m_numFrameBuffers;
   // }

    void VulkanRenderTaskManager::reclaim(){
        vkDestroyCommandPool(p_context->getDevice(),m_vkCommandPool,nullptr);
        for(size_t i =0;i<m_numFrameBuffers;i++){
        vkDestroySemaphore(p_context->getDevice(),renderFinishedSemaphores[i],nullptr);
        vkDestroySemaphore(p_context->getDevice(),imageAvailableSemaphores[i],nullptr);
        vkDestroyFence(p_context->getDevice(),commandBufferExecutingFences[i],nullptr);
//        vkDestroyFence(p_context->getDevice(),imageInUseFences[i],nullptr);
        }
    }

    void VulkanRenderTaskManager::reset() {
        m_currentFrameIndex = 0; 
    }

}
//
// Created by W on 1/20/21.
//

#include "willow/Vulkan/VulkanCommandInterface.hpp"
namespace wlo::wk {
    VulkanCommandInterface::VulkanCommandInterface(wlo::wk::VulkanRoot &root):m_root(root) {
        auto [GraphicsQueueIndex, PresentQueueIndex] = m_root.QueueFamilyIndices();
        vk::CommandPoolCreateInfo poolCreateInfo(vk::CommandPoolCreateFlags(), GraphicsQueueIndex);
        m_commandPool = m_root.Device().createCommandPoolUnique(poolCreateInfo);
        m_graphicsQueue = m_root.Device().getQueue(GraphicsQueueIndex, 0);

    }
    void VulkanCommandInterface::submitImmediately(std::function<void(vk::UniqueCommandBuffer&)> && function){
        vk::UniqueCommandBuffer commandBuffer = newCommandBuffer();

        commandBuffer->begin(vk::CommandBufferBeginInfo(vk::CommandBufferUsageFlagBits::eOneTimeSubmit));

        function(commandBuffer);

        commandBuffer->end();

        vk::UniqueFence submissionFence = m_root.Device().createFenceUnique(vk::FenceCreateInfo());
        vk::SubmitInfo         submitInfo({},{},commandBuffer.get());
        m_graphicsQueue.submit(submitInfo, submissionFence.get());
        while (vk::Result::eTimeout == m_root.Device().waitForFences(submissionFence.get(), VK_TRUE, FenceTimeout))
            ;
    }

    vk::UniqueCommandBuffer VulkanCommandInterface::newCommandBuffer() {
        return std::move(m_root.Device().allocateCommandBuffersUnique(vk::CommandBufferAllocateInfo(
                        m_commandPool.get(), vk::CommandBufferLevel::ePrimary, 1))
                                                                  .front());
    }

    void VulkanCommandInterface::submitGraphicsCommands(vk::SubmitInfo submitInfo, vk::UniqueFence & submitFence) {
        m_graphicsQueue.submit(submitInfo, submitFence.get());
    }
}
#pragma once
#include "VulkanSwapchain.hpp"
#include "vulkan_render_pass.hpp"
#include "vulkan_context.hpp"

#include "willow/dev-utils/Vulkan/wilovk.hpp"

namespace wlo{
        class VulkanFrameBufferArray {
            public:
                VulkanFrameBufferArray();
                void initialize(VkDevice device, const VulkanSwapchain& swapchain, const VulkanRenderPass& renderPass);
                void reclaim(VkDevice device) ;
                void reinitialize(VkDevice device, const VulkanSwapchain& swapchain, const VulkanRenderPass& renderPass);

                size_t size();
                VkFramebuffer operator [] (size_t i);
                ~VulkanFrameBufferArray();
            private:
                std::vector<VkFramebuffer> m_vkFrameBuffers;
                wlo::SharedPointer<VulkanRenderingContext> p_context;
                wlo::SharedPointer<VulkanSwapchain> p_swapchain;
                wlo::SharedPointer<VulkanRenderPass> p_renderPass;
                  

        };



}
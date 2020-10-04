#pragma once
#include "willow/dev-utils/Vulkan/wilovk.hpp"



namespace wlo{
    class VulkanRenderPass  {
        public:
            VulkanRenderPass (VkDevice device, VkFormat surfaceFormat);
            void reclaim(VkDevice device);
            VkRenderPass get() const;
            ~VulkanRenderPass();
        private:
            VkRenderPass m_vkRenderPass;

    };




}
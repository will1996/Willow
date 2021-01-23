//
// Created by W on 1/20/21.
//

#ifndef WILLOW_VULKANCOMMANDINTERFACE_H
#define WILLOW_VULKANCOMMANDINTERFACE_H

#include "VulkanRoot.hpp"
namespace wlo::wk {
    class VulkanCommandInterface {
    public:
        const uint64_t FenceTimeout = 100000000;
        VulkanCommandInterface(VulkanRoot &root);
        void submitImmediately(std::function<void(vk::UniqueCommandBuffer&)> && function);
        vk::UniqueCommandBuffer newCommandBuffer();
        void submitGraphicsCommands(vk::SubmitInfo, vk::UniqueFence &);
    private:
        wk::VulkanRoot& m_root;
        vk::UniqueCommandPool m_commandPool;
        vk::Queue m_graphicsQueue;
    };
}


#endif //WILLOW_VULKANCOMMANDINTERFACE_H

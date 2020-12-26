//
// Created by W on 12/3/20.
//

#ifndef WILLOW_VULKANMEMORYMANAGER_HPP
#define WILLOW_VULKANMEMORYMANAGER_HPP
#include "willow/rendering/Vulkan/VulkanRoot.hpp"
#include "willow/rendering/Vulkan/VulkanBuffer.hpp"
namespace wlo::wk {
    class VulkanMemoryManger {
        VulkanMemoryManger(const VulkanRoot &root);
        VulkanBuffer newBuffer(const byte * data);




    };
}
#endif //WILLOW_VULKANMEMORYMANAGER_HPP

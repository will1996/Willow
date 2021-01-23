//
// Created by W on 12/3/20.
//

#ifndef WILLOW_VULKANMEMORYMANAGER_HPP
#define WILLOW_VULKANMEMORYMANAGER_HPP
#include "willow/Vulkan/VulkanRoot.hpp"
#include "willow/Vulkan/VulkanBuffer.hpp"
#include "willow/Vulkan/VulkanCommandInterface.hpp"
#include"willow/rendering/DataLayout.hpp"
namespace wlo::wk {
    struct DeviceBuffer{
        vk::UniqueBuffer buffer;
        vk::UniqueDeviceMemory memory;
    };
    struct MappedBuffer:DeviceBuffer {
        byte* writePoint;
    };

    struct DeviceImage{
       vk::UniqueDeviceMemory  memory;
       vk::Extent3D imageExtent;
       vk::ImageLayout layout;
       vk::UniqueImage image;
       vk::UniqueImageView view;
    };


    class VulkanMemoryManger {
    public:
        VulkanMemoryManger(VulkanRoot &root,VulkanCommandInterface &commandInterface);

        MappedBuffer allocateMappedBuffer(const rendering::DataLayout& layout
                                          ,size_t count ,
                                          vk::BufferUsageFlagBits usage
                                          );

        DeviceBuffer allocateBuffer(const rendering::DataLayout& layout,
                                    size_t count,
                                    vk::BufferUsageFlagBits usage,
                                    vk::MemoryPropertyFlags properties = vk::MemoryPropertyFlagBits::eDeviceLocal
                                    );

        DeviceImage allocateImage(const rendering::DataLayout& pixelLayout,
                                  uint32_t width,
                                  uint32_t height,
                                  vk::ImageLayout imageLayout,
                                  vk::ImageUsageFlags usage
                                  );

        void copy(DeviceImage& to,DeviceBuffer& from);
    private:

        VulkanRoot &m_root;
        VulkanCommandInterface & m_commandInterface;
    };
}
#endif //WILLOW_VULKANMEMORYMANAGER_HPP

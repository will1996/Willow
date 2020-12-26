//
// Created by W on 12/3/20.
//
#include "willow/Vulkan/VulkanBuffer.hpp"
namespace wlo::wk {
    VulkanBuffer::VulkanBuffer(vk::Device & device ,
                               const byte * dataSource,
                                const vk::MemoryAllocateInfo allocInfo):
            m_device(device),
            m_readSource(dataSource)
    {
        createVkBuffer();
//        allocateVkBufferMemory(gpu);
        vkBindBufferMemory(m_device, m_vkBuffer, m_vkMemory, 0);
        vkMapMemory(m_device, m_vkMemory, 0, memSizeBytes(), 0, &m_writeLocation);
    }

    void VulkanBuffer::upload(size_t count,size_t offset) {
        void *write_ptr;
 //       memcpy(write_ptr, m_readSource, m_dataTargetReadSize * m_layout.memSize());
    }

    VkBuffer VulkanBuffer::get() {
        return m_vkBuffer;
    }

    void VulkanBuffer::reclaim() {
        vkUnmapMemory(m_device, m_vkMemory);
        vkDestroyBuffer(m_device, m_vkBuffer, nullptr);
        vkFreeMemory(m_device, m_vkMemory, nullptr);
    }

    size_t VulkanBuffer::memSizeBytes() {
//        return m_count * m_layout.memSize();
        return 0;
    }

    size_t VulkanBuffer::size() {
        return m_count;
    }


    void VulkanBuffer::createVkBuffer() {
        VkBufferCreateInfo bufferInfo{
                .size = memSizeBytes(),
                //.usage = m_bufferUsage,
                .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        };
       m_vkBuffer =  m_device.createBuffer(bufferInfo,nullptr);
    }

    void VulkanBuffer::allocateVkBufferMemory(const vk::PhysicalDevice & gpu){
        vk::MemoryRequirements memRequirements  = m_device.getBufferMemoryRequirements(m_vkBuffer);
        VkMemoryAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        //allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, m_memoryProperties);
         m_vkMemory = m_device.allocateMemory( allocInfo, nullptr);
    }

    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
        VkPhysicalDeviceMemoryProperties memProperties;
        //vkGetPhysicalDeviceMemoryProperties(p_context->getPhysicalDevice(), &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }

        throw std::runtime_error("failed to find suitable memory type!");
    }



}
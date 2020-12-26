#pragma once
#include"willow/Vulkan/VulkanRoot.hpp"


/*Vulkan buffer handles the creation and destruction of a vkBuffer object. Manages the upload of content.*/

namespace wlo::wk{

	 class VulkanBuffer {
		public:	
			VulkanBuffer(vk::Device & device,
                         const byte * data,
                         const vk::MemoryAllocateInfo allocInfo);
			void upload(size_t count,size_t offset = 0);
            VkBuffer get();

			void reclaim();
			size_t memSizeBytes() ;
			size_t size();
	private:
        void allocate();
		void createVkBuffer();

		void allocateVkBufferMemory(const vk::PhysicalDevice & gpu);
		
		uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
			VkPhysicalDeviceMemoryProperties memProperties;
//			vkGetPhysicalDeviceMemoryProperties(&memProperties);

			for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
				if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
					return i;
				}
			}

			throw std::runtime_error("failed to find suitable memory type!");
		}



		vk::Device m_device;
        const byte * m_readSource;
        void * m_writeLocation;
        vk::Buffer m_vkBuffer;

		//the size of the buffer  in elements
		size_t m_count;

		//the size of the content we will read from the CPU in elements
		size_t m_dataTargetReadSize;

        VkDeviceMemory m_vkMemory;

	};

}
#pragma once
#include "vulkan_context.hpp"
#include "willow/dev-utils/Vulkan/wilovk.hpp"



/*Vulkan buffer handles the creation and destruction of a vkBuffer object. Manages the upload of content.*/

namespace wlo{
	template <typename T> class VulkanBuffer {
		public:	
			VulkanBuffer(){ }

			virtual void initialize(VulkanRenderingContext* contextRef ){
				p_context = contextRef;
			}

			void allocate(size_t size,VkBufferUsageFlags usage, VkMemoryPropertyFlags properties){
				m_size = size;
				m_bufferUsage = usage;
				m_memoryProperties = properties;
				createVkBuffer();
				allocateVkBufferMemory();
                vkBindBufferMemory(p_context->getDevice(), m_vkBuffer, m_vkMemory, 0);
			}
			virtual void resize(size_t newsize) {
                vkDestroyBuffer(p_context->getDevice(), m_vkBuffer, nullptr);
			    vkFreeMemory(p_context->getDevice(),m_vkMemory,nullptr);
                m_size = newsize;
                createVkBuffer();
                allocateVkBufferMemory();
                vkBindBufferMemory(p_context->getDevice(), m_vkBuffer, m_vkMemory, 0);
			}

			virtual void setDataTarget(const T* data, size_t ammount , size_t offset = 0) {
				
				m_dataTarget = data+offset;
				m_dataTargetReadSize = ammount;
			};
			virtual void uploadDataTarget(){
                void* write_ptr;
                vkMapMemory(p_context->getDevice(), m_vkMemory, 0, memSizeBytes(), 0, &write_ptr);
                memcpy(write_ptr, m_dataTarget,  m_dataTargetReadSize*sizeof(T));
                vkUnmapMemory(p_context->getDevice(), m_vkMemory); 
			}
            VkBuffer get(){
                return m_vkBuffer;
            }

			void reclaim() {
                vkDestroyBuffer(p_context->getDevice(), m_vkBuffer, nullptr);
                vkFreeMemory(p_context->getDevice(), m_vkMemory, nullptr);
			}
			size_t memSizeBytes() {
					return m_size*sizeof(T);
			}
			size_t size(){
				return m_size;
			}
	private:
		void createVkBuffer() {
			VkBufferCreateInfo bufferInfo{};
			bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			bufferInfo.usage = m_bufferUsage;
			bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			bufferInfo.size = memSizeBytes();

			VkResult res = vkCreateBuffer(p_context->getDevice(), &bufferInfo, nullptr, &m_vkBuffer);
			vk::checkResult(res, "create buffer");
		}

		void allocateVkBufferMemory(){
			VkMemoryRequirements memRequirements;
			vkGetBufferMemoryRequirements(p_context->getDevice(), m_vkBuffer, &memRequirements);
			VkMemoryAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			allocInfo.allocationSize = memRequirements.size;
			allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, m_memoryProperties);

			VkResult res = vkAllocateMemory(p_context->getDevice(), &allocInfo, nullptr, &m_vkMemory);
			vk::checkResult(res, "allocate memory");
		}
		
		uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
			VkPhysicalDeviceMemoryProperties memProperties;
			vkGetPhysicalDeviceMemoryProperties(p_context->getPhysicalDevice(), &memProperties);

			for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
				if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
					return i;
				}
			}

			throw std::runtime_error("failed to find suitable memory type!");
		}

        VulkanRenderingContext* p_context;
		//the size of the buffer  in elements
		size_t m_size;	


		//a pointer to the content we will read from the CPU
		const T* m_dataTarget;
		//the size of the content we will read from the CPU in elements
		size_t m_dataTargetReadSize;

        VkDeviceMemory m_vkMemory;
        VkBuffer m_vkBuffer;
	
		VkMemoryPropertyFlags m_memoryProperties ;
		VkBufferUsageFlags  m_bufferUsage ;
	};

}
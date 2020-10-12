#include "willow/rendering/Vulkan/vulkan_index_buffer.hpp"



namespace wlo{



	VulkanIndexBuffer::VulkanIndexBuffer()
	{ }

	void VulkanIndexBuffer::initialize(VulkanRenderingContext* contextRef)
	{
		m_buffer.initialize(contextRef);
	}

	void VulkanIndexBuffer::allocate(size_t nElements)
	{
		VkBufferUsageFlags usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
		VkMemoryPropertyFlags memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
		m_buffer.allocate(nElements, usage, memoryProperties);
	}
	void VulkanIndexBuffer::fill(const std::vector<uint32_t>& data, size_t nElements, size_t offset)
	{
		m_buffer.setDataTarget(data.data(), nElements, offset);
		m_buffer.uploadDataTarget();
	}
	
	void VulkanIndexBuffer::reclaim()
	{
		m_buffer.reclaim();
	}
	
	size_t VulkanIndexBuffer::size()
	{
		return m_buffer.size();
	}
	
	size_t VulkanIndexBuffer::memSizeBytes()
	{
		return m_buffer.memSizeBytes();
	}
	
	VkBuffer VulkanIndexBuffer::get(){
		return m_buffer.get();
	}

    void VulkanIndexBuffer::reallocate(size_t nElements) {
	    m_buffer.resize(nElements);
    }

}

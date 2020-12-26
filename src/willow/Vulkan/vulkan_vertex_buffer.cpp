#include "willow/rendering/Vulkan/vulkan_vertex_buffer.hpp"



namespace wlo{

	


	VulkanVertexBuffer::VulkanVertexBuffer()
	{
	}


	void VulkanVertexBuffer::initialize(VulkanRenderingContext* contextRef){
		m_data.initialize(contextRef);
	}
	
	void VulkanVertexBuffer::allocate(size_t nElements)
	{
        VkBufferUsageFlags usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		VkMemoryPropertyFlags memoryProperties = VK_MEMORY_PROPERTY_HOST_COHERENT_BIT|VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
		m_data.allocate(nElements,usage, memoryProperties );
	}

	void VulkanVertexBuffer::fill(const std::vector<Vertex3D>& data, size_t nElements, size_t offset )
	{
		m_data.setDataTarget(data.data(), nElements, offset);
		m_data.uploadDataTarget();
	}

	void VulkanVertexBuffer::reclaim()
	{
		m_data.reclaim();
	}

	size_t VulkanVertexBuffer::size()
	{
		return m_data.size();
	}

	size_t VulkanVertexBuffer::memSizeBytes()
	{
		return m_data.memSizeBytes();
	}

	VkBuffer VulkanVertexBuffer::get()
	{
		return m_data.get();
	}

    void VulkanVertexBuffer::reallocate(size_t nElements) {
	    m_data.resize(nElements);
    }

}

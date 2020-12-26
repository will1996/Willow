#include "willow/rendering/Vulkan/vulkan_uniform_buffer_array.hpp"
namespace wlo{

        VulkanUniformBufferArray::VulkanUniformBufferArray():
        m_layout{ {RenderDataType::fmat4x4, "model"}, {RenderDataType::fmat4x4, "view"}, {RenderDataType::fmat4x4, "projection"} },
        m_descriptorSetLayoutBinding{}
        {
        };
        
		void VulkanUniformBufferArray::initialize(VulkanRenderingContext* contextRef, size_t numFrameBuffers)
		{
            m_uboData.resize(numFrameBuffers);
            m_numFrameBuffers = numFrameBuffers;
            m_buffers.resize(numFrameBuffers);
         for(auto buffer : m_buffers){
             buffer.initialize(contextRef);
         }
            allocateBuffers(contextRef);

         m_descriptorSetLayoutBinding.binding = 0;
         m_descriptorSetLayoutBinding.descriptorCount = 1;
         m_descriptorSetLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
         m_descriptorSetLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

         VkDescriptorSetLayoutCreateInfo layoutInfo{};
         layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
         layoutInfo.bindingCount =1 ;
         layoutInfo.pBindings = &m_descriptorSetLayoutBinding;

         VkResult res = vkCreateDescriptorSetLayout(contextRef->getDevice(),&layoutInfo,nullptr, &m_descriptorSetLayout);
         vk::checkResult(res, "create descriptor set layout");

		}

		void VulkanUniformBufferArray::upload(size_t frameBufferIndex) {
            assert(frameBufferIndex<m_numFrameBuffers);
            m_buffers[frameBufferIndex].uploadDataTarget();
        } 
		void VulkanUniformBufferArray::reclaim(){
            for(size_t i=0;i<m_buffers.size();i++){
                m_buffers[i].reclaim();
                }
        } 
		 void VulkanUniformBufferArray::setView(size_t frameBufferIndex, const glm::mat4x4 & viewMatrix) {
            assert(frameBufferIndex<m_numFrameBuffers);
             m_uboData[frameBufferIndex].view = viewMatrix;
         };
		 void VulkanUniformBufferArray::setProjection( size_t frameBufferIndex, const glm::mat4x4& projectionMatrix){
            assert(frameBufferIndex<m_numFrameBuffers);
             m_uboData[frameBufferIndex].projection = projectionMatrix;
         };
		 void VulkanUniformBufferArray::setModel(size_t frameBufferIndex,  const glm::mat4x4& modelMatrix) {
            assert(frameBufferIndex<m_numFrameBuffers);
             m_uboData[frameBufferIndex].model = modelMatrix;
         };
		
		 VkDescriptorSetLayout VulkanUniformBufferArray::getDescriptorSetLayout()
		 {
             return  m_descriptorSetLayout;
		 }

         std::vector<VkDescriptorBufferInfo> VulkanUniformBufferArray::getBufferInfos() {
             std::vector<VkDescriptorBufferInfo> bufferInfos(m_buffers.size());

             for (size_t i = 0; i < m_buffers.size(); i++) {
                 VkDescriptorBufferInfo bufInfo{};
                 bufInfo.buffer = m_buffers[i].get();
                 bufInfo.offset = 0;
                 bufInfo.range = sizeof(UniformBufferObject);
                 bufferInfos[i] = bufInfo;
             }

             return bufferInfos;
         
         }

        void VulkanUniformBufferArray::allocateBuffers(VulkanRenderingContext* p_context){
            VkBufferUsageFlags usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
            VkMemoryPropertyFlags memoryProperties= VK_MEMORY_PROPERTY_HOST_COHERENT_BIT|VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
            for(size_t i = 0; i<m_buffers.size();i++){
                m_buffers[i].initialize(p_context);
                m_buffers[i].allocate(sizeof(UniformBufferObject),usage, memoryProperties);
                m_buffers[i].setDataTarget(&m_uboData[i],1);
                }

           
        }



}

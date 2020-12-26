#pragma once
#include <glm/glm.hpp>
#include "vulkan_context.hpp"
#include "willow/dev-utils/Vulkan/wilovk.hpp"
#include "VulkanBuffer.hpp"
#include "willow/root/wilo_dev_core.hpp"
#include "willow/rendering/wilo_render_data_layout.hpp"
#include "vulkan_uniform_buffer_array.hpp"
namespace wlo{

    class VulkanUniformBufferArray {
        public:
        VulkanUniformBufferArray();

         void initialize(VulkanRenderingContext*, size_t numFrameBuffers);

		 void setView(size_t NumFrameBuffers, const glm::mat4x4 & viewMatrix) ;
		 void setProjection(size_t NumFrameBuffers,  const glm::mat4x4& projectionMatrix);
		 void setModel(size_t NumFrameBuffers,  const glm::mat4x4& modelMatrix) ;
         VkDescriptorSetLayout getDescriptorSetLayout();
         std::vector<VkDescriptorBufferInfo> getBufferInfos();
		 void upload(size_t frameBufferIndex) ;					
		 void reclaim() ;
            
        private: 

        void allocateBuffers(VulkanRenderingContext*);
        


        
        struct UniformBufferObject{
            glm::mat4x4 model; 
            glm::mat4x4 view; 
            glm::mat4x4 projection; 
            };

    
        size_t m_numFrameBuffers;
        RenderDataLayout m_layout;
        VkDescriptorSetLayout m_descriptorSetLayout;        
        VkDescriptorSetLayoutBinding m_descriptorSetLayoutBinding;

        std::vector<VulkanBuffer<UniformBufferObject> > m_buffers;
        std::vector<UniformBufferObject> m_uboData;



    };



}

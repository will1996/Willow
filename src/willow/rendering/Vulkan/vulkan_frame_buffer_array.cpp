#include "willow/rendering/Vulkan/vulkan_frame_buffer_array.hpp"

namespace wlo{
    

	VulkanFrameBufferArray::VulkanFrameBufferArray()
	{
	}

	void VulkanFrameBufferArray::initialize(VkDevice device, const VulkanSwapchain& swapchain, const VulkanRenderPass& renderPass)
	{
        m_vkFrameBuffers.resize(swapchain.getImageViews().size());

        for(size_t i =0; i<m_vkFrameBuffers.size(); i++){
            VkImageView attachments[]  = {
                swapchain.getImageViews()[i]
            };

            VkFramebufferCreateInfo bufferInfo{};

            bufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;

            bufferInfo.attachmentCount = 1;
            bufferInfo.pAttachments = attachments;

            bufferInfo.renderPass = renderPass.get();

            bufferInfo.width = swapchain.getSwapSurfaceExtent().width;
            bufferInfo.height = swapchain.getSwapSurfaceExtent().height;
            
            bufferInfo.layers = 1;


            VkResult res = vkCreateFramebuffer(device,&bufferInfo,nullptr,&m_vkFrameBuffers[i]);

            vk::checkResult(res,"create frame buffer"+std::to_string(i));
        }

	}


    VkFramebuffer VulkanFrameBufferArray :: operator [] (size_t i){
            return m_vkFrameBuffers[i];
    }


	void VulkanFrameBufferArray::reclaim(VkDevice device)
	{
        for(size_t i=0;i<m_vkFrameBuffers.size();i++)
        vkDestroyFramebuffer(device,m_vkFrameBuffers[i],nullptr);
	}


	void VulkanFrameBufferArray::reinitialize(VkDevice device, const VulkanSwapchain& swapchain, const VulkanRenderPass& renderPass)
	{
        reclaim(device);
        initialize(device, swapchain, renderPass);
       
	}

	size_t VulkanFrameBufferArray::size() {
         return m_vkFrameBuffers.size();
     }
    VulkanFrameBufferArray::~VulkanFrameBufferArray(){}


}
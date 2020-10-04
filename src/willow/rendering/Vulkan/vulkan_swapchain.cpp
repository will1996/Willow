#include "willow/rendering/Vulkan/vulkan_swapchain.hpp"
#include <algorithm>


namespace wlo{

	VulkanSwapchain::VulkanSwapchain() {}

	void VulkanSwapchain::initialize( const VulkanRenderingContext* contextRef, uint32_t width, uint32_t height){
		p_context = contextRef;
		m_supportDetails = p_context->getSwapChainSupportDetails();
		m_swapSurfaceExtent = {width, height};
		createVkSwapchain();
		getSwapchainImages();
		createSwapchainImageViews();
		WILO_CORE_INFO("Vulkan swapchain initialized!")
	}
	void VulkanSwapchain::reinitialize( uint32_t width, uint32_t height){
		m_supportDetails = p_context->getSwapChainSupportDetails();
		m_swapSurfaceExtent = { width, height };
		reclaim();
		createVkSwapchain();
		getSwapchainImages();
		createSwapchainImageViews();
		WILO_CORE_INFO("Vulkan swapchain reInitialized!")
	}
	void VulkanSwapchain::reclaim(){
		for(auto imageView: m_imageViews){
			vkDestroyImageView(p_context->getDevice(),imageView,nullptr);
		}
		vkDestroySwapchainKHR(p_context->getDevice(),m_swapchain,nullptr);
	}

	VkSwapchainKHR VulkanSwapchain::get(){
		return m_swapchain;
	};
	VkExtent2D  VulkanSwapchain::getSwapSurfaceExtent() const{
		return m_swapSurfaceExtent;
	}
	VkFormat 	VulkanSwapchain::getSwapSurfaceFormat() const{
		return m_swapSurfaceFormat.format;
	}
	const std::vector<VkImageView>& VulkanSwapchain::getImageViews() const{
		return m_imageViews;
	}

	void VulkanSwapchain::createVkSwapchain(){
		 chooseSwapChainExtent();//populate m_swapSurfaceFormat
		 chooseSwapSurfaceFormat();//populate m_swapSurfaceFormat

		VkPresentModeKHR presentMode = choosePresentMode();



		VkSwapchainCreateInfoKHR swapChain_info = {};
		swapChain_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapChain_info.imageFormat = m_swapSurfaceFormat.format;//what is the image format 
		swapChain_info.imageColorSpace = m_swapSurfaceFormat.colorSpace;//what is the color space
		swapChain_info.imageExtent = m_swapSurfaceExtent;//what is the resolution of the frames
		swapChain_info.presentMode = presentMode;//how are we buffering frames before they are presented
		swapChain_info.imageArrayLayers = 1;
		swapChain_info.minImageCount = m_supportDetails.capabilities.minImageCount+1;
		//TODO make imageUsage switch to transferDSTBit for such things as, post_processing
		swapChain_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		//setup swapchain image sharing mode depending if it will be accessed from multiple
		//threads or not. Queue families opperate asychnonously, so we need to tell Vulakn
		//to use the images safely asychonously if we have more than one queue family for
		//presentation and graphics 
		vk::QueueFamilyIndices queueFamiliyIndices = vk::findQueueFamilies(p_context->getPhysicalDevice(),p_context->getSurface());
		uint32_t indicesArray[] = {queueFamiliyIndices.graphicsFamily.value(),queueFamiliyIndices.presentFamily.value()};
		if(queueFamiliyIndices.presentFamily.value()==queueFamiliyIndices.graphicsFamily.value()){
			swapChain_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			swapChain_info.queueFamilyIndexCount = 0;
			swapChain_info.pQueueFamilyIndices = nullptr;
		}else{
			swapChain_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			swapChain_info.queueFamilyIndexCount = 2;
			swapChain_info.pQueueFamilyIndices = indicesArray;
		}


		swapChain_info.preTransform = m_supportDetails.capabilities.currentTransform;
		swapChain_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapChain_info.presentMode = presentMode;
		swapChain_info.clipped = true;

		swapChain_info.oldSwapchain = VK_NULL_HANDLE;
		swapChain_info.surface = p_context->getSurface();

		VkResult res = vkCreateSwapchainKHR(p_context->getDevice(),&swapChain_info, nullptr,&m_swapchain);
		vk::checkResult(res,"create swapchain");


	}

	void VulkanSwapchain::getSwapchainImages(){
			uint32_t imageCount;
			vkGetSwapchainImagesKHR(p_context->getDevice(),m_swapchain,&imageCount,nullptr);
			m_images.resize(imageCount);
			vkGetSwapchainImagesKHR(p_context->getDevice(),m_swapchain,&imageCount,m_images.data());
	}
	void VulkanSwapchain::createSwapchainImageViews(){
		m_imageViews.resize(m_images.size());
		for(size_t i =0;i<m_imageViews.size();i++){
			VkImageViewCreateInfo viewInfo{};
			viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			viewInfo.image = m_images[i];
			viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;//these are essentially textures
			viewInfo.format = m_swapSurfaceFormat.format;
			//TODO apply swizzling intelligently	
			viewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			viewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			viewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			viewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

			viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			viewInfo.subresourceRange.baseArrayLayer = 0;//only one layer for the image
			viewInfo.subresourceRange.layerCount = 1;
			viewInfo.subresourceRange.baseMipLevel = 0;//only one mip level (the full scale image)
			viewInfo.subresourceRange.levelCount = 1;

			VkResult res = vkCreateImageView(p_context->getDevice(),&viewInfo,nullptr, &m_imageViews[i]);
			vk::checkResult(res,"create Image view");

		}
	}
	void VulkanSwapchain::chooseSwapSurfaceFormat(){

			for(const auto & format : m_supportDetails.formats){
					if(format.format==VK_FORMAT_B8G8R8A8_SRGB && 
								format.colorSpace==VK_COLOR_SPACE_SRGB_NONLINEAR_KHR){
										 m_swapSurfaceFormat = format;
										 return;
								}
			}
			m_swapSurfaceFormat =  m_supportDetails.formats[0];
	}

	VkPresentModeKHR   VulkanSwapchain::choosePresentMode(){
		for(const auto & presentMode : m_supportDetails.presentModes)	{
			if(presentMode == VK_PRESENT_MODE_MAILBOX_KHR)
				return presentMode;
		}
		return VK_PRESENT_MODE_FIFO_KHR;
	}

	void 		   VulkanSwapchain::chooseSwapChainExtent(){
			//If the width is not equal to UINT32_MAX, then we can't select the extent
			if(m_supportDetails.capabilities.currentExtent.width != UINT32_MAX){
				m_swapSurfaceExtent= m_supportDetails.capabilities.currentExtent;
				return;
			}
			VkExtent2D extent = m_swapSurfaceExtent;
			
			extent.width = m_supportDetails.capabilities.minImageExtent.width > extent.width ? m_supportDetails.capabilities.minImageExtent.width : extent.width;

			extent.height = m_supportDetails.capabilities.minImageExtent.height > extent.height? m_supportDetails.capabilities.minImageExtent.width : extent.height;

			m_swapSurfaceExtent = extent;
	}
}
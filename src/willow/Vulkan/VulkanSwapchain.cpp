#include "willow/Vulkan/VulkanSwapchain.hpp"
#include "GLFW/glfw3.h"
#include <algorithm>


namespace wlo::wk{
    VulkanSwapchain::VulkanSwapchain( wk::VulkanRoot& root,wlo::SharedPointer<Window> window):
    m_root(root),
    m_swapSurfaceExtent{window->getInfo().m_width,window->getInfo().m_height}

    {
        WILO_CORE_INFO("Creating swapchain...");
        window->permit<WindowResized,VulkanSwapchain,&VulkanSwapchain::resize>(this);
        VkSurfaceKHR mainWindowSurface;
        VkResult res = glfwCreateWindowSurface(m_root.Instance(),(GLFWwindow*)window->getNativeWindow(),nullptr,&mainWindowSurface );
        if(res!=VK_SUCCESS)
            throw std::runtime_error("something went wrong with GLFW");
        m_surface = vk::SurfaceKHR(mainWindowSurface);
        if(! m_root.supportSurface(m_surface))
            throw std::runtime_error("CreatedVulkan root does not support swapchain present surface");
        createVkSwapchain();
        createImageViews();
        WILO_CORE_INFO("Swapchain created!");
    }


	void VulkanSwapchain::resize(const wlo::WindowResized & msg){
		m_swapSurfaceExtent = vk::Extent2D{static_cast<uint32_t> (msg.content.width),static_cast<uint32_t> (msg.content.height)};
		reclaim();
		createVkSwapchain();
		createImageViews();
		WILO_CORE_INFO("Vulkan swapchain reInitialized!")
	}

	void VulkanSwapchain::reclaim(){
//		for(auto imageView: m_imageViews){
//			vkDestroyImageView(p_context->getDevice(),imageView,nullptr);
//		}
//		vkDestroySwapchainKHR(p_context->getDevice(),m_swapchain,nullptr);
	}

	vk::SwapchainKHR VulkanSwapchain::get(){
		return m_vkSwapchain;
	};
	vk::Extent2D  VulkanSwapchain::getSwapSurfaceExtent() const{
		return m_swapSurfaceExtent;
	}
	vk::Format 	VulkanSwapchain::getSwapSurfaceFormat() const{
		return m_swapSurfaceFormat.format;
	}

	void VulkanSwapchain::createVkSwapchain(){
		 chooseSwapChainExtent();//populate m_swapSurfaceFormat
		 chooseSwapSurfaceFormat();//populate m_swapSurfaceFormat

        vk::SurfaceCapabilitiesKHR surfaceCapabilities = m_root.PhysicalDevice().getSurfaceCapabilitiesKHR(m_surface);
        vk::CompositeAlphaFlagBitsKHR compositeAlpha =
                ( surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::ePreMultiplied )
                ? vk::CompositeAlphaFlagBitsKHR::ePreMultiplied
                : ( surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::ePostMultiplied )
                  ? vk::CompositeAlphaFlagBitsKHR::ePostMultiplied
                  : ( surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::eInherit )
                    ? vk::CompositeAlphaFlagBitsKHR::eInherit
                    : vk::CompositeAlphaFlagBitsKHR::eOpaque;

        vk::SwapchainCreateInfoKHR swapchainInfo;
            swapchainInfo.flags=vk::SwapchainCreateFlagsKHR();
            swapchainInfo.imageExtent = m_swapSurfaceExtent;
            swapchainInfo.minImageCount = m_root.PhysicalDevice().getSurfaceCapabilitiesKHR(m_surface).minImageCount;
            swapchainInfo.imageFormat = m_swapSurfaceFormat.format;
            swapchainInfo.presentMode = choosePresentMode();
            swapchainInfo.imageColorSpace = vk::ColorSpaceKHR::eSrgbNonlinear;
            swapchainInfo.surface = m_surface;
            swapchainInfo.clipped = true;
            swapchainInfo.preTransform = vk::SurfaceTransformFlagBitsKHR::eIdentity;
            swapchainInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;
            swapchainInfo.compositeAlpha = compositeAlpha;
            swapchainInfo.imageArrayLayers = 1;
            swapchainInfo.pNext = nullptr;

        auto [graphicsIndex, presentIndex] = m_root.QueueFamilyIndices();
        presentIndex = graphicsIndex;
        uint32_t queueFamilyIndices[2]  = {graphicsIndex,presentIndex};//pack indices into an array to appease vulkan



        swapchainInfo.pQueueFamilyIndices = queueFamilyIndices;

        if(presentIndex!=graphicsIndex){
            swapchainInfo.imageSharingMode = vk::SharingMode::eConcurrent;
            swapchainInfo.queueFamilyIndexCount = 2;
        }
        else{
            swapchainInfo.imageSharingMode = vk::SharingMode::eExclusive;
            swapchainInfo.queueFamilyIndexCount = 1;
        }
        m_vkSwapchain =  m_root.Device().createSwapchainKHR(swapchainInfo);
	}

	void VulkanSwapchain::createImageViews(){
	    m_images = m_root.Device().getSwapchainImagesKHR(m_vkSwapchain);
		m_imageViews.resize(m_images.size());
		for(size_t i =0;i<m_imageViews.size();i++){
			vk::ImageViewCreateInfo viewInfo{};
			viewInfo.image = m_images[i];
			viewInfo.viewType = vk::ImageViewType::e2D;
			viewInfo.format = m_swapSurfaceFormat.format;
			//TODO apply swizzling intelligently	
			viewInfo.components.r = vk::ComponentSwizzle::eIdentity;
			viewInfo.components.g = vk::ComponentSwizzle::eIdentity;
			viewInfo.components.b = vk::ComponentSwizzle::eIdentity;
			viewInfo.components.a = vk::ComponentSwizzle::eIdentity;
			viewInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
			viewInfo.subresourceRange.baseArrayLayer = 0;//only one layer for the image
			viewInfo.subresourceRange.layerCount = 1;
			viewInfo.subresourceRange.baseMipLevel = 0;//only one mip level (the full scale image)
			viewInfo.subresourceRange.levelCount = 1;
			m_imageViews[i] = m_root.Device().createImageView(viewInfo);
		}
	}
	void VulkanSwapchain::chooseSwapSurfaceFormat(){
        std::vector<vk::SurfaceFormatKHR> formats = m_root.PhysicalDevice().getSurfaceFormatsKHR(m_surface);
        m_swapSurfaceFormat = (formats[0].format==vk::Format::eUndefined) ? vk::Format::eB8G8R8A8Unorm : formats[0].format;
	}
//
	vk::PresentModeKHR   VulkanSwapchain::choosePresentMode(){
//		for(const auto & presentMode : m_supportDetails.presentModes)	{
//			if(presentMode == VK_PRESENT_MODE_MAILBOX_KHR)
//				return presentMode;
//		}
		return  vk::PresentModeKHR::eFifo;
	}

	void 		   VulkanSwapchain::chooseSwapChainExtent(){

	    vk::SurfaceCapabilitiesKHR surfaceCapabilities = m_root.PhysicalDevice().getSurfaceCapabilitiesKHR(m_surface);

	    if(surfaceCapabilities.currentExtent==std::numeric_limits<uint32_t>::max()){
            m_swapSurfaceExtent.width = std::clamp(m_swapSurfaceExtent.width,surfaceCapabilities.minImageExtent.width,surfaceCapabilities.maxImageExtent.width);
            m_swapSurfaceExtent.height = std::clamp(m_swapSurfaceExtent.height,surfaceCapabilities.minImageExtent.height,surfaceCapabilities.maxImageExtent.height);
	    }else{
	        m_swapSurfaceExtent = surfaceCapabilities.currentExtent;
	    }
	}

    const vk::ImageView &VulkanSwapchain::getCurView() const {
        return m_imageViews[m_curFrameIndex];
    }

    VulkanSwapchain::~VulkanSwapchain() {
	    m_root.Device().destroy(m_vkSwapchain);
        m_root.Instance().destroy(m_surface);
        for(auto & imageView : m_imageViews)
            m_root.Device().destroy(imageView);

    }

}
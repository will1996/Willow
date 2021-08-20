#include "willow/Vulkan/VulkanSwapchain.hpp"
#include "GLFW/glfw3.h"
#include <algorithm>


namespace wlo::wk{
   /*
    * Swapchains are a piece of machinery that alternately presents images to the screen for viewing,
    * and returns the images to the renderer for drawing.
    *
    *
    *
    *
    */



    VulkanSwapchain::VulkanSwapchain( wk::VulkanRoot& root,wlo::Window& window):
    m_root(root),
    m_swapSurfaceExtent{window.getInfo().m_width,window.getInfo().m_height},
    m_window(window)

    {

        //create an oldschool VKSurface because glfw is a hoe
        initialize();
    }

    void VulkanSwapchain::initialize() {
        VkSurfaceKHR mainWindowSurface;
        VkResult res = glfwCreateWindowSurface(m_root.Instance(),(GLFWwindow*)m_window.getNativeWindow(),nullptr,&mainWindowSurface );
        if(res!=VK_SUCCESS)
            throw std::runtime_error("something went wrong with GLFW");
        m_surface = vk::SurfaceKHR(mainWindowSurface);//wrap that in a much better surface object
        m_swapSurfaceExtent = vk::Extent2D{m_window.getInfo().m_width,m_window.getInfo().m_height};
        if(! m_root.supportSurface(m_surface)) throw std::runtime_error("CreatedVulkan root does not support swapchain present surface");
        createVkSwapchain();
        createImageViews();
        createDepthBuffers();
    }

    void VulkanSwapchain::reclaim(){
        m_root.Device().destroy(m_vkSwapchain);
        m_root.Device().free(m_depthBufferMemory);
        m_root.Device().destroy(m_depthView);
        m_root.Device().destroy(m_depthImage);
        m_root.Instance().destroy(m_surface);
        for(auto & imageView : m_imageViews)
            m_root.Device().destroy(imageView);
    }

	void VulkanSwapchain::resize(){
		m_swapSurfaceExtent = vk::Extent2D{m_window.getInfo().m_width,m_window.getInfo().m_height};
		reclaim();
		initialize();
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

	    if(surfaceCapabilities.currentExtent.width==std::numeric_limits<uint32_t>::max()){
            m_swapSurfaceExtent.width = std::clamp(m_swapSurfaceExtent.width,surfaceCapabilities.minImageExtent.width,surfaceCapabilities.maxImageExtent.width);
            m_swapSurfaceExtent.height = std::clamp(m_swapSurfaceExtent.height,surfaceCapabilities.minImageExtent.height,surfaceCapabilities.maxImageExtent.height);
	    }else{
	        m_swapSurfaceExtent = surfaceCapabilities.currentExtent;
	    }
	}


    VulkanSwapchain::~VulkanSwapchain() {
	    reclaim();
    }


    void VulkanSwapchain::createDepthBuffers(){
	    const vk::Format     depthFormat      = vk::Format::eD16Unorm;
        vk::FormatProperties formatProperties = m_root.PhysicalDevice().getFormatProperties( depthFormat );
        vk::ImageTiling tiling;
        if ( formatProperties.linearTilingFeatures & vk::FormatFeatureFlagBits::eDepthStencilAttachment )
        {
            tiling = vk::ImageTiling::eLinear;
        }
        else if ( formatProperties.optimalTilingFeatures & vk::FormatFeatureFlagBits::eDepthStencilAttachment )
        {
        tiling = vk::ImageTiling::eOptimal;
        }
        else
        {
        throw std::runtime_error( "DepthStencilAttachment is not supported for D16Unorm depth format." );
        }

        vk::ImageCreateInfo depthImageCreateInfo( vk::ImageCreateFlags(),
                                             vk::ImageType::e2D,
                                             depthFormat,
                                             vk::Extent3D( m_swapSurfaceExtent, 1 ),
                                             1,
                                             1,
                                             vk::SampleCountFlagBits::e1,
                                             tiling,
                                             vk::ImageUsageFlagBits::eDepthStencilAttachment );
        m_depthImage = m_root.Device().createImage(depthImageCreateInfo);

        vk::PhysicalDeviceMemoryProperties memoryProperties   = m_root.PhysicalDevice().getMemoryProperties();
        vk::MemoryRequirements             memoryRequirements = m_root.Device().getImageMemoryRequirements( m_depthImage );

        uint32_t                           typeBits           = memoryRequirements.memoryTypeBits;//simple memory requirement check
        uint32_t                           typeIndex          = uint32_t( ~0 );
        for ( uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++ )
        {
            if ( ( typeBits & 1 ) &&
                 ( ( memoryProperties.memoryTypes[i].propertyFlags & vk::MemoryPropertyFlagBits::eDeviceLocal ) ==
                   vk::MemoryPropertyFlagBits::eDeviceLocal ) )
            {
                typeIndex = i;
                break;
            }
            typeBits >>= 1;
        }

        assert( typeIndex != uint32_t( ~0 ) );
       m_depthBufferMemory = m_root.Device().allocateMemory(vk::MemoryAllocateInfo{memoryRequirements.size,typeIndex});

       m_root.Device().bindImageMemory(m_depthImage,m_depthBufferMemory,0);
        vk::ComponentMapping componentMapping(
                vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA );
        vk::ImageSubresourceRange subResourceRange( vk::ImageAspectFlagBits::eDepth, 0, 1, 0, 1 );

        m_depthView = m_root.Device().createImageView( vk::ImageViewCreateInfo( vk::ImageViewCreateFlags(),
                                                                                                m_depthImage,
                                                                                                vk::ImageViewType::e2D,
                                                                                                depthFormat,
                                                                                                componentMapping,
                                                                                                subResourceRange ) );
    }

    const std::vector<vk::ImageView> &VulkanSwapchain::getSwapSurfaceViews() {
	    return m_imageViews;
    }

    const vk::ImageView &VulkanSwapchain::getDepthImageView() {
	    return m_depthView;
    }


}
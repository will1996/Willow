#include "willow/dev-utils/Vulkan/vkswapchain.hpp"
#include "willow/dev-utils/Vulkan/vkdevice.hpp"
#include "willow/dev-utils/Vulkan/vkutil.hpp"

namespace wlo{
  namespace wk{

    VkExtent2D chooseSwapExtent(uint32_t width, uint32_t height,const VkSurfaceCapabilitiesKHR& capabilities) {
        if (capabilities.currentExtent.width != UINT32_MAX) {
            return capabilities.currentExtent;
        } else {

            VkExtent2D actualExtent = { width, height };

            actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
            actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

            return actualExtent;
        }
    }

    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
        for (const auto& availablePresentMode : availablePresentModes) {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                return availablePresentMode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
        for (const auto& availableFormat : availableFormats) {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return availableFormat;
            }
        }

        return availableFormats[0];
    }
    
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device,VkSurfaceKHR surface) { 
      SwapChainSupportDetails details;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

        if (formatCount != 0) {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

        if (presentModeCount != 0) {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
        }

        return details;
    }



    Swapchain::Swapchain(Device device, VkSurfaceKHR surface){ 
        m_device = device;
        m_surface = surface; 
      }

    Swapchain::~Swapchain(){  }
    
  void Swapchain::initialize( uint32_t width,uint32_t height){
      SwapChainSupportDetails swapChainSupport = querySwapChainSupport(m_device.physical(),m_surface);

      VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
      VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
      VkExtent2D extent = chooseSwapExtent(width, height,swapChainSupport.capabilities);

      uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
      if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
          imageCount = swapChainSupport.capabilities.maxImageCount;
      }

      VkSwapchainCreateInfoKHR createInfo = {};
      createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
      createInfo.surface = m_surface;

      createInfo.minImageCount = imageCount;
      createInfo.imageFormat = surfaceFormat.format;
      createInfo.imageColorSpace = surfaceFormat.colorSpace;
      createInfo.imageExtent = extent;
      createInfo.imageArrayLayers = 1;
      createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

      QueueFamilyIndices indices = findQueueFamilies(m_device.physical(),m_surface);
      uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

      if (indices.graphicsFamily != indices.presentFamily) {
          createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
          createInfo.queueFamilyIndexCount = 2;
          createInfo.pQueueFamilyIndices = queueFamilyIndices;
      } else {
          createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
      }

      createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
      createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
      createInfo.presentMode = presentMode;
      createInfo.clipped = VK_TRUE;

      checkResult(vkCreateSwapchainKHR(m_device.logical(), &createInfo, nullptr, &m_swapchain), "create swap chain!");
      

      vkGetSwapchainImagesKHR(m_device.logical(), m_swapchain, &imageCount, nullptr);
      m_swapchainImages.resize(imageCount);
      vkGetSwapchainImagesKHR(m_device.logical(), m_swapchain, &imageCount, m_swapchainImages.data());

      m_swapchainImageFormat = surfaceFormat.format;
      m_swapchainExtent = extent;

      m_swapchainImageViews.resize(m_swapchainImages.size());

      for (size_t i = 0; i < m_swapchainImages.size(); i++) {
          VkImageViewCreateInfo createInfo = {};
          createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
          createInfo.image = m_swapchainImages[i];
          createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
          createInfo.format = m_swapchainImageFormat;
          createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
          createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
          createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
          createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
          createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
          createInfo.subresourceRange.baseMipLevel = 0;
          createInfo.subresourceRange.levelCount = 1;
          createInfo.subresourceRange.baseArrayLayer = 0;
          createInfo.subresourceRange.layerCount = 1;

          checkResult(
           vkCreateImageView(m_device.logical(),
                              &createInfo,
                              nullptr,
                              &m_swapchainImageViews[i]) ,

                   "create swapchain image views");
        }
  }
  void Swapchain::reclaim(){
  
        for (auto framebuffer : m_swapchainFramebuffers) {
            vkDestroyFramebuffer(m_device.logical(), framebuffer, nullptr);
        }
        for (auto imageView : m_swapchainImageViews) {
            vkDestroyImageView(m_device.logical(), imageView, nullptr);
        }
        vkDestroySwapchainKHR(m_device.logical(), m_swapchain, nullptr);
  }

  VkExtent2D Swapchain::getExtent(){
    return m_swapchainExtent; 
  } 
  
  uint32_t Swapchain::getNumImages(){
    return static_cast<uint32_t>(m_swapchainImages.size()); 
  } 
  
  VkFormat Swapchain::getFormat(){
    return m_swapchainImageFormat;
  }
  
  }

}

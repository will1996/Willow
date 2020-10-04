#include "willow/dev-utils/Vulkan/vkdevice.hpp"

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <array>
#include <set>
//helper functions for device class
wlo::vk::SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) {
  wlo::vk::SwapChainSupportDetails details;

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

 bool checkDeviceExtensionSupport(VkPhysicalDevice device) {
      const std::vector<const char*> deviceExtensions = {
          VK_KHR_SWAPCHAIN_EXTENSION_NAME
      };
      uint32_t extensionCount;
      vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

      std::vector<VkExtensionProperties> availableExtensions(extensionCount);
      vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

      std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

      for (const auto& extension : availableExtensions) {
          requiredExtensions.erase(extension.extensionName);
      }

      return requiredExtensions.empty();
  }


//device class definitions
namespace wlo{
  namespace vk{


  Device::Device(){
    //creating resources with the constructor would be fine, but, 
    //as a design choice I've decided that if we're explicitly deleting
    //resources, we may as well explicitly request them too. 
    #ifdef NDEBUG
     m_enableValidation = false;
    #else
     m_enableValidation = true;
    #endif
  }
 void  Device::initialize(VkInstance instance, VkSurfaceKHR surface){
      f_selectPhysicalDevice(instance, surface);
      f_createLogicalDevice(surface);

  }
 void  Device::reclaim(){
    
        vkDestroyDevice(m_logicalDevice, nullptr);
  }

  Device::~Device(){
  //do nothing, we can't trust any automatic garbage collection, 
  //if our Device goes away when we're using it life will suck and 
  //we won't know why. so all resources are freed explicitly using reclaim
  
  }

bool Device::f_physicalDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface) {
   QueueFamilyIndices indices = findQueueFamilies(device,surface);

    bool extensionsSupported = checkDeviceExtensionSupport(device);

    bool swapChainAdequate = false;
    if (extensionsSupported) {
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device,surface);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(device,&deviceFeatures);
    return indices.isComplete() && extensionsSupported && swapChainAdequate
      &&deviceFeatures.samplerAnisotropy;
}



  void Device::f_selectPhysicalDevice(VkInstance instance, VkSurfaceKHR surface){
  
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0) {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    for (const auto& device : devices) {
        if (f_physicalDeviceSuitable(device,surface)) {
            m_physicalDevice = device;
            break;
        }
    }

    if (m_physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("failed to find a suitable GPU!");
    }
  }


  void Device::f_createLogicalDevice(VkSurfaceKHR surface){
    QueueFamilyIndices indices = findQueueFamilies(m_physicalDevice,surface);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo = {};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures = {};
    deviceFeatures.samplerAnisotropy = VK_TRUE;
    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();

    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

    if (m_enableValidation) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_logicalDevice) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
    }
    m_queues.resize(2);//create space for the present and graphics queues
    vkGetDeviceQueue(m_logicalDevice, indices.graphicsFamily.value(), 0, &m_queues[m_graphicsQueueIndex]);
    vkGetDeviceQueue(m_logicalDevice, indices.presentFamily.value(), 0, &m_queues[m_presentQueueIndex]);
       
  }
  
  

  VkQueue Device::getGraphicsQueue(){
    if(m_queues.empty())
      throw std::runtime_error("::vk::device must be initialized before its graphics queue is used");
    return m_queues[m_graphicsQueueIndex];
  }

  VkQueue Device::getPresentQueue(){
    if(m_queues.empty())
      throw std::runtime_error("::vk::device must be initialized before its present queue is used");
    return m_queues[m_presentQueueIndex];
  }
   VkDevice& Device::logical(){
    return m_logicalDevice; 
   }
   VkPhysicalDevice& Device::physical(){
    return m_physicalDevice; 
     }
  bool Device::getValidationState(){
    return m_enableValidation; 
  }
 void Device::setValidationState(bool set){
    m_enableValidation = set;
 }

}}

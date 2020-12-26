//
// Created by W on 11/29/20.
//
#include "willow/Vulkan/VulkanRoot.hpp"
#include "willow/Vulkan/VulkanSetup.hpp"
#include <GLFW/glfw3.h>

namespace wlo::wk{
    VulkanRoot::VulkanRoot(std::initializer_list<wlo::rendering::Renderer::Features> features,Window * window, bool debugUtils) {
        m_instance = wk::createInstance(getInstanceLayers(features),getInstanceExtensions(features),debugUtils);
        WILO_CORE_INFO("VulkanRoot created Instance");
        if(debugUtils)
            m_debugMessenger = wk::createDebugMessenger(m_instance);
        //create a temporary surface such that the physical and logical devices can be set up with knowledge of the window format
        VkSurfaceKHR mainWindowSurface;
        VkResult res = glfwCreateWindowSurface(m_instance,(GLFWwindow*)window->getNativeWindow(),nullptr,&mainWindowSurface );
        if(res!=VK_SUCCESS)
            throw std::runtime_error("something went wrong with GLFW");

        selectPhysicalDevice(mainWindowSurface);
        createLogicalDevice(features,mainWindowSurface);
        //now, destroy this surface, the main swapchain will own the window.
        m_instance.destroy(mainWindowSurface);
    }

    std::vector<const char *> VulkanRoot::getInstanceExtensions(std::initializer_list<wlo::rendering::Renderer::Features> features) {
        std::vector<const char *> requestedExtensions; requestedExtensions.reserve(features.size());
        for (auto & feature : features )
            requestedExtensions.insert(requestedExtensions.end(),m_extensionLookup[feature].begin(),m_extensionLookup[feature].end());

        return requestedExtensions;
    }

    std::vector<const char *> VulkanRoot::getInstanceLayers(std::initializer_list<wlo::rendering::Renderer::Features> features) {
        std::vector<const char *> requestedLayers; requestedLayers.reserve(features.size());
        for (auto & feature : features )
            requestedLayers.insert(requestedLayers.end(), m_layerLookup[feature].begin(), m_layerLookup[feature].end());

        return requestedLayers;
    }


    vk::Instance &VulkanRoot::Instance() { return m_instance; }
    vk::Device &VulkanRoot::Device() { return m_device; }
    vk::PhysicalDevice &VulkanRoot::PhysicalDevice() { return m_physicalDevice; }

    VulkanRoot::~VulkanRoot() {
        m_device.destroy();
        m_instance.destroy(m_debugMessenger);
        m_instance.destroy();
    }


    bool physicalDeviceSupportsSurface(vk::SurfaceKHR surface, const vk::PhysicalDevice & device){
            auto supportedFormats = device.getSurfaceFormatsKHR(surface);
            WILO_CORE_INFO("for this device/surface combination vulkan supports: {0} formats",supportedFormats.size());
            auto supportedPresentModes = device.getSurfacePresentModesKHR(surface);
        WILO_CORE_INFO("for this device/surface combination vulkan supports: {0} presentmodes",supportedPresentModes.size());
        return !supportedFormats.empty() && !supportedPresentModes.empty();
    }


    void VulkanRoot::selectPhysicalDevice(vk::SurfaceKHR surface) {
        auto candidates = m_instance.enumeratePhysicalDevices();
        if(candidates.empty())
            throw std::runtime_error("Vulkan bootstrap failed, no vulkan capable physical devices found");
        int best = -1;
        int acceptable = -1;
      for (size_t i = 0; i < candidates.size(); i++){
          vk::PhysicalDeviceProperties properties = candidates[i].getProperties();
          vk::PhysicalDeviceFeatures features = candidates[i].getFeatures();
              if(physicalDeviceSupportsSurface(surface, candidates[i])) {
                  WILO_CORE_INFO("Vulkan implementation found Physical device which supports window surface");
                  if (properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu) {
                      best = i;
                  } else {
                      acceptable = i;
                  }
              }
          }

        if(best <0){
            WILO_CORE_WARNING("unable to find ideal vulkan device, some features may not work");
            if(acceptable < 0){
                WILO_CORE_ERROR("Basic graphics functionality unsupported, renderer setup failure");
                throw std::runtime_error("Failed to find usable vulkan device");
            }else{
                WILO_CORE_INFO("Vulkan Root settled for Integrated GPU, but basic vulkan support present");
                m_physicalDevice = candidates[acceptable];
            }
        }
        else{
            WILO_CORE_INFO("Vulkan Root using Dedicated Graphics Card");
            m_physicalDevice = candidates[best];
        }


    }



    std::vector<const char *>
    VulkanRoot::getDeviceExtensions(std::initializer_list<wlo::rendering::Renderer::Features> features) {
        std::vector<const char *> extensionNames;
        extensionNames.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
        for(auto & feature: features){
            for(auto & name : m_deviceExtensionLookup[feature])
                extensionNames.push_back(name);
        }
        return extensionNames;
    }

    uint32_t findGraphicsQueueFamilyIndex( std::vector<vk::QueueFamilyProperties> const & queueFamilyProperties )
    {
        // get the first index into queueFamiliyProperties which supports graphics
        size_t graphicsQueueFamilyIndex = std::distance(
                queueFamilyProperties.begin(),
                std::find_if(
                        queueFamilyProperties.begin(), queueFamilyProperties.end(), []( vk::QueueFamilyProperties const & qfp ) {
                            return qfp.queueFlags & vk::QueueFlagBits::eGraphics;
                        } ) );
        assert( graphicsQueueFamilyIndex < queueFamilyProperties.size() );

        return static_cast<uint32_t>( graphicsQueueFamilyIndex );
    }



   std::pair<uint32_t,uint32_t> getQueueFamilyIndices( vk::PhysicalDevice physicalDevice,vk::SurfaceKHR surface){
       // determine a queueFamilyIndex that suports present
       // first check if the graphicsQueueFamiliyIndex is good enough
       auto queueFamilyProperties = physicalDevice.getQueueFamilyProperties();


    uint32_t graphicsQueueFamilyIndex = findGraphicsQueueFamilyIndex(queueFamilyProperties);
      uint32_t  presentQueueFamilyIndex =
               physicalDevice.getSurfaceSupportKHR( static_cast<uint32_t>( graphicsQueueFamilyIndex ), surface )
               ? graphicsQueueFamilyIndex
               : queueFamilyProperties.size();
       if ( presentQueueFamilyIndex == queueFamilyProperties.size() )
       {
           // the graphicsQueueFamilyIndex doesn't support present -> look for an other family index that supports both
           // graphics and present
           for ( size_t i = 0; i < queueFamilyProperties.size(); i++ )
           {
               if ( ( queueFamilyProperties[i].queueFlags & vk::QueueFlagBits::eGraphics ) &&
                    physicalDevice.getSurfaceSupportKHR( static_cast<uint32_t>( i ), surface ) )
               {
                   graphicsQueueFamilyIndex = i;
                   presentQueueFamilyIndex  = i;
                   break;
               }
           }
           if ( presentQueueFamilyIndex == queueFamilyProperties.size() )
           {
               // there's nothing like a single family index that supports both graphics and present -> look for an other
               // family index that supports present
               for ( size_t i = 0; i < queueFamilyProperties.size(); i++ )
               {
                   if ( physicalDevice.getSurfaceSupportKHR( static_cast<uint32_t>( i ), surface ) )
                   {
                       presentQueueFamilyIndex = i;
                       break;
                   }
               }
           }
       }
       if ( ( graphicsQueueFamilyIndex == queueFamilyProperties.size() ) ||
            ( presentQueueFamilyIndex == queueFamilyProperties.size() ) )
       {
           throw std::runtime_error( "Could not find a queue for graphics or present -> terminating" );
       }
       return {graphicsQueueFamilyIndex,presentQueueFamilyIndex};
   }


    void VulkanRoot::createLogicalDevice(std::initializer_list<wlo::rendering::Renderer::Features> features,
                                         vk::SurfaceKHR surface) {
       auto extensionNames = getDeviceExtensions(features);
       auto [graphicsQueueIndex, presentQueueIndex] = getQueueFamilyIndices(m_physicalDevice,surface);
       m_queueFamilyIndices.graphics = graphicsQueueIndex;
       m_queueFamilyIndices.present = presentQueueIndex;
       float queuePriorities = 0.0f;
       if(graphicsQueueIndex==presentQueueIndex) {
           vk::DeviceQueueCreateInfo graphicsAndPresentQueueCreateInfo(vk::DeviceQueueCreateFlags(), graphicsQueueIndex,
                                                                       1, &queuePriorities);
           vk::DeviceCreateInfo deviceCreateInfo(vk::DeviceCreateFlags(), graphicsAndPresentQueueCreateInfo, {},
                                                 extensionNames, {});
           m_device = m_physicalDevice.createDevice(deviceCreateInfo);
           WILO_CORE_INFO("VulkanRoot created a logical device with one queue for graphics and present combined");
       }else{
           vk::DeviceQueueCreateInfo graphicsQueueCreateInfo(vk::DeviceQueueCreateFlags(), graphicsQueueIndex,
                                                                       1, &queuePriorities);

           vk::DeviceQueueCreateInfo presentQueueCreateInfo(vk::DeviceQueueCreateFlags(), presentQueueIndex,
                                                             1, &queuePriorities);
            std::array<vk::DeviceQueueCreateInfo,2> QueueInfos = {graphicsQueueCreateInfo,presentQueueCreateInfo};
           vk::DeviceCreateInfo deviceCreateInfo(vk::DeviceCreateFlags(), QueueInfos, {},
                                                 extensionNames, {});
           m_device = m_physicalDevice.createDevice(deviceCreateInfo);
           WILO_CORE_INFO("VulkanRoot created a logical device with two separate queues; one queue for graphics and  one queue for present ");
       }
    }

    bool VulkanRoot::supportSurface(vk::SurfaceKHR & surface) {
        m_physicalDevice.getSurfaceSupportKHR(m_queueFamilyIndices.present,surface);
        return physicalDeviceSupportsSurface(surface,m_physicalDevice);
    }


}
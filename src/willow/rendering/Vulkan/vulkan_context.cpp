#include "willow/rendering/Vulkan/vulkan_context.hpp"
#include "willow/root/wilo_dev_core.hpp"
#include <set>
#include<vulkan/vulkan.h>
#include <GLFW/glfw3.h>

namespace wlo {
    VulkanRenderingContext::VulkanRenderingContext()
        :
        m_physicalDevice(VK_NULL_HANDLE),
        m_device(VK_NULL_HANDLE),
        m_shadersPath(WILO_APIS_PATH)//get the base path from the engine
    {
        m_shadersPath+="Vulkan/rendering/shaders/";//append the vulkan specific path
    }
    
    void VulkanRenderingContext::initialize(wlo::SharedPointer<Window> windowRef, VulkanRenderingContext::Info info) {
        m_enableValidationLayers = info.enableValidationLayers;
        p_window = windowRef;
        createInstance();
        WILO_CORE_INFO("Vulkan Instance Initialized!");

        if (m_enableValidationLayers)
            setupDebugging();

        createSurface();
        WILO_CORE_INFO("Vulkan surface created");
        selectPhysicalDevice();
        WILO_CORE_INFO("Vulkan Physical Device selected");
        createLogicalDevice();
        WILO_CORE_INFO("Vulkan Logical Device created");
        WILO_CORE_INFO("Vulkan Context Initialized");
    }


    void VulkanRenderingContext::reclaim(){
        if (m_enableValidationLayers) {
            vk::DestroyDebugUtilsMessengerEXT(m_instance, m_debugMessenger, nullptr);
        }
        vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
        vkDestroyDevice(m_device, nullptr);
        vkDestroyInstance(m_instance, nullptr);
        WILO_CORE_INFO("Vulkan  context shutdown correctly!");
    }



    VkInstance VulkanRenderingContext::getInstance() const 
    {
        return m_instance;
     }

    VkPhysicalDevice VulkanRenderingContext::getPhysicalDevice() const 
    {
        return m_physicalDevice;
    }

    VkDevice VulkanRenderingContext::getDevice() const 
    {
        return m_device;
    }

    VkSurfaceKHR VulkanRenderingContext::getSurface()  const 
    {
        return m_surface;
    }

    std::string VulkanRenderingContext::getShadersPath() const
    {
        return m_shadersPath;
    }

    VulkanRenderingContext::~VulkanRenderingContext() {

    }

    //TODO make the required extensions change with windowing API
    std::vector<const char*> VulkanRenderingContext::getInstanceExtensions() {
        std::vector<const char*> instanceExtensions;
        uint32_t extensionCount;

        const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&extensionCount);
        instanceExtensions.insert(instanceExtensions.begin(), glfwExtensions, glfwExtensions + extensionCount);

        if (m_enableValidationLayers)
            instanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);


        return instanceExtensions;
    }



    bool validateLayers(std::vector<const char*> wantedLayerNames) {
        uint32_t availibleLayerCount;
        vkEnumerateInstanceLayerProperties(&availibleLayerCount, nullptr);
        std::vector <VkLayerProperties> availableLayerProperties(availibleLayerCount);
        vkEnumerateInstanceLayerProperties(&availibleLayerCount, availableLayerProperties.data());

        for (const char* wantedLayerName : wantedLayerNames) {
            bool found = false;
            for (VkLayerProperties availableLayer : availableLayerProperties) {
                if (strcmp(availableLayer.layerName, wantedLayerName)) {
                    std::stringstream succstr;
                    succstr << "requested VulkanLayer: " << wantedLayerName << " FOUND!" << std::endl;
                    WILO_CORE_INFO(succstr.str());
                    found = true;
                }
            }
            if (!found) {
                std::stringstream errstr;
                errstr << "requested VulkanLayer: " << wantedLayerName << " Not found, vulkan likely not supported on the platform" << std::endl;
                WILO_CORE_ERROR(errstr.str());
                assert(found);
                return false;
            }
        }
        return true;
    }
    std::vector<const char*> VulkanRenderingContext::getInstanceLayers()
    {
        std::vector<const char*> instanceLayers;
        const std::vector<const char*> validationLayers = {
            "VK_LAYER_KHRONOS_validation"
        };



        if (m_enableValidationLayers)
            instanceLayers.insert(instanceLayers.end(), validationLayers.begin(), validationLayers.end());


        validateLayers(instanceLayers);
        return instanceLayers;
    }





    //used in createInstance, and setup debugging
    VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData
    ) {
        std::stringstream errmsg;
        errmsg << "[Vulkan Validation layer] " << pCallbackData->pMessage << std::endl;
        WILO_CORE_ERROR(errmsg.str());
        return VK_FALSE;
    }

    void VulkanRenderingContext::createInstance() {
        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.engineVersion = WILO_ENGINE_VERSION;
        appInfo.pEngineName = "";
        appInfo.apiVersion = VK_API_VERSION_1_1;
        appInfo.pApplicationName = "wilo_app";
        appInfo.applicationVersion = 0;


        VkInstanceCreateInfo instanceInfo = {};

        instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceInfo.pApplicationInfo = &appInfo;



        std::vector<const char*> instanceExtensions = getInstanceExtensions();

        instanceInfo.ppEnabledExtensionNames = instanceExtensions.data();
        instanceInfo.enabledExtensionCount = instanceExtensions.size();


        std::vector<const char*> instanceLayers = getInstanceLayers();
        instanceInfo.ppEnabledLayerNames = instanceLayers.data();
        instanceInfo.enabledLayerCount = instanceLayers.size();

        if (m_enableValidationLayers) {
            //make the instance create it's own debug messenger to notify us on insstance creation issues
            VkDebugUtilsMessengerCreateInfoEXT debuggerInfo = {};
            debuggerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
            debuggerInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
            debuggerInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
            debuggerInfo.pfnUserCallback = debugCallback;
            debuggerInfo.pUserData = nullptr;

            instanceInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debuggerInfo;
        }

        VkResult res = vkCreateInstance(&instanceInfo, nullptr, &m_instance);
        vk::checkResult(res, "create instance");
    }


    void VulkanRenderingContext::setupDebugging() {
        VkDebugUtilsMessengerCreateInfoEXT info = {};
        info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        info.pfnUserCallback = debugCallback;
        info.pUserData = nullptr;
        VkResult res = vk::CreateDebugUtilsMessengerEXT(m_instance, &info, nullptr, &m_debugMessenger);
        vk::checkResult(res, "create debug messenger");
    }

    void VulkanRenderingContext::createSurface() {
        if (p_window->getInfo().API != wlo::WindowingAPICode::GLFW) {
            throw std::runtime_error("Vulkan currently only supports GLFW");
        }

        GLFWwindow* glfwWindow = (GLFWwindow*)(p_window->getNativeWindow());
        glfwSetWindowTitle(glfwWindow, "This totally worked lmao");
        assert(glfwWindow);//if this is null we're humped

        VkResult res = glfwCreateWindowSurface(m_instance, glfwWindow, nullptr, &m_surface);
        vk::checkResult(res, "create Surface");
    }


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
    //public facing version of querySwapChainSupportDetails;
      vk::SwapChainSupportDetails VulkanRenderingContext::getSwapChainSupportDetails() const {
          return querySwapChainSupport(m_physicalDevice,m_surface);
      }

    std::vector<const char*> VulkanRenderingContext::getDeviceExtensions()
    {
       std::vector<const char*> deviceExtensions = {
          VK_KHR_SWAPCHAIN_EXTENSION_NAME
      };
       return deviceExtensions;
    }

    bool checkDeviceExtensionSupport(VkPhysicalDevice device, std::vector < const char*> deviceExtensions) {
      uint32_t extensionCount;
      vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

      std::vector<VkExtensionProperties> availableExtensions(extensionCount);
      vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

      std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

      for (const auto& extension : availableExtensions) {
          std::cout << extension.extensionName << std::endl;
          requiredExtensions.erase(extension.extensionName);
      }

      return requiredExtensions.empty();
  }

bool VulkanRenderingContext::physicalDeviceIsSuitable(VkPhysicalDevice device ){
    vk::QueueFamilyIndices queueFamilies =  vk::findQueueFamilies(device, m_surface);
    bool swapChainSupportAdequate = false;
    bool extensionsSupported = checkDeviceExtensionSupport(device,getDeviceExtensions());
    if(extensionsSupported){
        vk::SwapChainSupportDetails swapChainSupportDetails = querySwapChainSupport(device, m_surface);
        swapChainSupportAdequate = !swapChainSupportDetails.formats.empty() && !swapChainSupportDetails.presentModes.empty();
    }
    return queueFamilies.isComplete()&&extensionsSupported && swapChainSupportAdequate;
}


void VulkanRenderingContext::selectPhysicalDevice(){
    uint32_t deviceCount;
    vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());
    
    if (devices.size() == 0) {
        WILO_CORE_ERROR("No Vulkan Capable devices found");
        throw std::runtime_error("no vulkan devices found, willo unsupported");
    }


    for(VkPhysicalDevice device : devices){
        if(physicalDeviceIsSuitable(device)){
            m_physicalDevice = device;
            break;
        }
    }

    if(m_physicalDevice== VK_NULL_HANDLE){
        WILO_CORE_ERROR("No Vulkan devices supporting all requested device features found");
        throw std::runtime_error("no vulkan devices found, willo unsupported");
    }
}

/*
create the logical device from the previously created physical device, and surface.
create the device Queues for graphics, and presentation 
*/
void VulkanRenderingContext::createLogicalDevice() {
    //get the queue family indices from the physical device with the information from the surface. 
    vk::QueueFamilyIndices indices = vk::findQueueFamilies(m_physicalDevice, m_surface);
     std::set<uint32_t> uniqueQueueFamilyIndices = { indices.graphicsFamily.value(), indices.presentFamily.value() };

    //specify required queue family infos based on the fetched queueFamilyIndices, we only need one queue per unique index
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    float queuePriority = 1.0f;
      for (uint32_t queueFamilyIndex: uniqueQueueFamilyIndices){
    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;
    queueCreateInfo.pNext = nullptr;
    queueCreateInfo.flags = 0;
    queueCreateInfos.push_back(queueCreateInfo);
     }


    //specify device creation info using the queue Create Infos, and the deviceFeatures. 
    VkDeviceCreateInfo deviceInfo{};
    deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    //set the device extensions
    std::vector<const char* > deviceExtensions = getDeviceExtensions();
    deviceInfo.enabledExtensionCount = deviceExtensions.size();
    deviceInfo.ppEnabledExtensionNames = deviceExtensions.data();
    
    //specify special features for the device
    VkPhysicalDeviceFeatures deviceFeatures{};
    deviceInfo.pEnabledFeatures = &deviceFeatures;

    deviceInfo.queueCreateInfoCount = queueCreateInfos.size();
    deviceInfo.pQueueCreateInfos = queueCreateInfos.data();
    //note that we are omitting enabled layer count, and enabled layers, this is fine for us
    //DBGMKR -- if logical device creation fails on old devices, this could be part of the reason fix would be: fetch validation layers for devices and put them here

    deviceInfo.flags = 0;
    deviceInfo.pNext = nullptr;
    //creat the device 
    VkResult res = vkCreateDevice(m_physicalDevice, &deviceInfo, nullptr, &m_device);
    vk::checkResult(res, "create logical device");
    //fetch the queues created on the logical device
    vkGetDeviceQueue(m_device, indices.graphicsFamily.value(), 0, &m_graphicsQueue);
    vkGetDeviceQueue(m_device, indices.presentFamily.value(), 0, &m_presentQueue);

}

}
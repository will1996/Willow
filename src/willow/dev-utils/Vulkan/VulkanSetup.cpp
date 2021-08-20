//
// Created by W on 10/22/20.
//

#include "willow/Vulkan/VulkanSetup.hpp"
#include "willow/root/Root.hpp"
#include "willow/window/Window.hpp"
#include  "GLFW/glfw3.h"

PFN_vkCreateDebugUtilsMessengerEXT  pfnVkCreateDebugUtilsMessengerEXT;
PFN_vkDestroyDebugUtilsMessengerEXT pfnVkDestroyDebugUtilsMessengerEXT;
VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugUtilsMessengerEXT( VkInstance                                 instance,
                                                               const VkDebugUtilsMessengerCreateInfoEXT * pCreateInfo,
                                                               const VkAllocationCallbacks *              pAllocator,
                                                               VkDebugUtilsMessengerEXT *                 pMessenger )
{
    return pfnVkCreateDebugUtilsMessengerEXT( instance, pCreateInfo, pAllocator, pMessenger );
}

VKAPI_ATTR void VKAPI_CALL vkDestroyDebugUtilsMessengerEXT( VkInstance                    instance,
                                                            VkDebugUtilsMessengerEXT      messenger,
                                                            VkAllocationCallbacks const * pAllocator )
{
    return pfnVkDestroyDebugUtilsMessengerEXT( instance, messenger, pAllocator );
}

namespace wlo::wk{
    VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
    if(messageType == VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT) {
        std::cout<<"[Vulkan General Info] "<< pCallbackData->pMessage;
    }
    else if(messageType == VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT) {
        std::cout<<"[Vulkan Validation Error] "<< pCallbackData->pMessage;
    }
    else if(messageSeverity>=VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
        std::cout<<"[Vulkan Warning] "<< pCallbackData->pMessage;
    }
    else {
        std::cout<<"[Vulkan General Info] "<< pCallbackData->pMessage;
    }

    return VK_FALSE;
}

    std::vector<const char *>
    validateRequestedExtensions(std::vector<const char *> requestedExtensions) {
        std::vector<const char*> finalExtensions; finalExtensions.reserve(requestedExtensions.size());
        auto availableExtensions = vk::enumerateInstanceExtensionProperties();
        for(const char * name : requestedExtensions) {
            auto iter = std::find_if(availableExtensions.begin(), availableExtensions.end(),
                                     [&](const vk::ExtensionProperties &ep) {
                                         return strcmp(name, ep.extensionName) == 0;
                                     });
            if(iter==availableExtensions.end())
                std::cout<<"FAILED TO LOAD EXTENSION ASSOCIATED FUNCTIONALITY UNAVAILABLE "<<name<<std::endl;
            else
                finalExtensions.push_back(name);
        }
        return finalExtensions;
    }

    std::vector<const char *>
    validateRequestedLayers(std::vector<const char *> layers) {
        std::vector<const char *> finalLayers; finalLayers.reserve(layers.size());
        for (const char * layer : layers)
            if(layer!=nullptr) {
                finalLayers.push_back(layer);

            } else{
                std::cout<<"VulkanBackend not configured for requested Feature, startup failed"<<std::endl;
                assert(false);
            }


        return finalLayers;
    }

    vk::Instance createInstance( std::vector<const char* > layerNames,std::vector<const char* > extensionNames,bool debugging){

        vk::ApplicationInfo appInfo("WillowGame",
                                    0,
                                    "Willow",
                                    VK_API_VERSION_1_2);
        glfwInit();
        uint32_t glfwCount;
        const char ** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwCount);
        extensionNames.insert(extensionNames.end(),glfwExtensions,glfwExtensions+glfwCount);

       if(!debugging) {
           auto validLayerNames = validateRequestedLayers(layerNames);
           auto validExtensionNames = validateRequestedExtensions(extensionNames);
           vk::InstanceCreateInfo instanceCreateInfo( vk::InstanceCreateFlags(), &appInfo, validLayerNames, validExtensionNames );
           auto instance = vk::createInstance(instanceCreateInfo );
           return instance;

       }else {
           layerNames.push_back("VK_LAYER_KHRONOS_validation");
//           layerNames.push_back("VK_LAYER_LUNARG_assistant_layer");
           extensionNames.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

           vk::StructureChain<vk::InstanceCreateInfo, vk::DebugUtilsMessengerCreateInfoEXT> InstanceAndDebugInfo = {
                   vk::InstanceCreateInfo(vk::InstanceCreateFlags(), &appInfo, layerNames, extensionNames),
                   vk::DebugUtilsMessengerCreateInfoEXT({},
                   vk::DebugUtilsMessageSeverityFlagsEXT ( vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError),
                   vk::DebugUtilsMessageTypeFlagsEXT (vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation),
                    &debugCallback
                   )
           };
           auto instance = vk::createInstance(
                   InstanceAndDebugInfo.get<vk::InstanceCreateInfo>()
           );
           return instance;
       }
    }

    vk::DebugUtilsMessengerEXT createDebugMessenger(const vk::Instance & instance) {

        pfnVkCreateDebugUtilsMessengerEXT =
                reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>( instance.getProcAddr( "vkCreateDebugUtilsMessengerEXT" ) );
        if ( !pfnVkCreateDebugUtilsMessengerEXT )
        {
            std::cout << "GetInstanceProcAddr: Unable to find pfnVkCreateDebugUtilsMessengerEXT function." << std::endl;
            exit( 1 );
        }

        pfnVkDestroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
                instance.getProcAddr( "vkDestroyDebugUtilsMessengerEXT" ) );
        if ( !pfnVkDestroyDebugUtilsMessengerEXT )
        {
            std::cout << "GetInstanceProcAddr: Unable to find pfnVkDestroyDebugUtilsMessengerEXT function." << std::endl;
            exit( 1 );
        }
        vk::DebugUtilsMessageSeverityFlagsEXT severityFlags(
                vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
                vk::DebugUtilsMessageSeverityFlagBitsEXT::eError);
        vk::DebugUtilsMessageTypeFlagsEXT messageTypeFlags(vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
                                                           vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
                                                           vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation);
        return   instance.createDebugUtilsMessengerEXT( vk::DebugUtilsMessengerCreateInfoEXT(
                {}, severityFlags, messageTypeFlags, &debugCallback));
    }

    vk::SurfaceKHR createSurface(const vk::Instance& instance, SharedPointer<Window> wind) {
        GLFWwindow* glfwWindow = (GLFWwindow*)(wind->getNativeWindow());
        VkSurfaceKHR surface;
        glfwCreateWindowSurface(instance,glfwWindow,nullptr, &surface);
        return surface;
    }




    vk::PhysicalDevice selectGPU(vk::Instance instance,VkSurfaceKHR surface) {
         auto deviceList = instance.enumeratePhysicalDevices();
         assert(!deviceList.empty());
         for( auto device : deviceList){
             auto familyProperties = device.getQueueFamilyProperties();
             size_t graphicsIdx = familyProperties.size(), presentIdx = familyProperties.size();
             for(size_t i = 0; i< familyProperties.size(); i++) {
                 auto queueFamilyProp  = familyProperties[i];
                 if ((queueFamilyProp.queueFlags & vk::QueueFlagBits::eGraphics) == vk::QueueFlagBits::eGraphics)
                     graphicsIdx = i;
                 if(device.getSurfaceSupportKHR(uint32_t(i), surface))
                     presentIdx = i;

                 if(presentIdx==graphicsIdx){

                     return device;
                 }

             }
         }

        throw std::runtime_error("VULKAN FAILED TO BE SETUP");

    }

    vk::Device createDevice(vk::Instance) {
        return vk::Device();
    }

    vk::RenderPassCreateInfo createDefaultRenderPassInfo(vk::Format imageFormat) {
        std::array<vk::AttachmentDescription, 2> attachmentDescriptions;
        //color attachments (i.e the color image that's going to the screen)
        attachmentDescriptions[0] = vk::AttachmentDescription(vk::AttachmentDescriptionFlags(),
                                                              imageFormat,
                                                              vk::SampleCountFlagBits::e1,
                                                              vk::AttachmentLoadOp::eClear,
                                                              vk::AttachmentStoreOp::eStore,
                                                              vk::AttachmentLoadOp::eDontCare,
                                                              vk::AttachmentStoreOp::eDontCare,
                                                              vk::ImageLayout::eUndefined,
                                                              vk::ImageLayout::ePresentSrcKHR);
        //depth attachment (the depth buffer used for coloring)
        attachmentDescriptions[1] = vk::AttachmentDescription(vk::AttachmentDescriptionFlags(),
                                                              vk::Format::eD16Unorm,
                                                              vk::SampleCountFlagBits::e1,
                                                              vk::AttachmentLoadOp::eClear,
                                                              vk::AttachmentStoreOp::eDontCare,
                                                              vk::AttachmentLoadOp::eDontCare,
                                                              vk::AttachmentStoreOp::eDontCare,
                                                              vk::ImageLayout::eUndefined,
                                                              vk::ImageLayout::eDepthStencilAttachmentOptimal);

        vk::AttachmentReference colorReference(0, vk::ImageLayout::eColorAttachmentOptimal);
        vk::AttachmentReference depthReference(1, vk::ImageLayout::eDepthStencilAttachmentOptimal);
        vk::SubpassDescription  subpass(
                vk::SubpassDescriptionFlags(), vk::PipelineBindPoint::eGraphics, {}, colorReference, {}, &depthReference);

        return vk::RenderPassCreateInfo(vk::RenderPassCreateFlags(), attachmentDescriptions, subpass);
    }


}


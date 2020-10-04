#include "willow/dev-utils/Vulkan/vkcore.hpp"
#include "willow/root/wilo_dev_core.hpp"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <cstring>
#include <cstdlib>
#include<sstream>
#include <cstdint>
#include <array>
#include <set>
using std::cerr;
using std::endl;

namespace wlo{
  namespace vk{
    
VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    std::cout << "creating debug messenger ...." << std::endl;
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
    std::stringstream ss;
    ss<<"VALIDATION LAYER:"<<endl;
    if(messageType == VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT)
      ss<<"General"<<endl;
    else if(messageType == VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT)
      ss<<"Validation"<<endl;
    else
      ss<<"Performace"<<endl;
    if(messageSeverity>=VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
      ss << "ERROR " ;
    else
      ss<< "WARNING " ;

     ss<< pCallbackData->pMessage <<std::endl;
     ss<<"user data size:"<<sizeof(pUserData)<<endl;
     std::cerr<<ss.str()<<endl;

    return VK_FALSE;
}

void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
}

bool checkValidationLayerSupport(){

        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char* layerName : validationLayers) {
            bool layerFound = false;

            for (const auto& layerProperties : availableLayers) {
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound) {
                return false;
            }
        }

        return true;


}

//  Core::Core(){
//      #ifdef NDEBUG
//      m_enableValidation = false;
//      #else
//      m_enableValidation = true;
//      #endif
//  }
//
//  Core::~Core(){
//  //do nothing, all resource allocation and de-allocation is managed from initialized and reclaim 
//  
//  }
//  void Core::initialize(coreCreateInfo info){
//    f_createInstance(info);
//    f_setupDebugMessenging();
//  }
//  void Core::reclaim(){
//    DestroyDebugUtilsMessengerEXT(m_instance,m_debugMessenger, nullptr);
//    vkDestroyInstance(m_instance, nullptr);
//     
//  }
//
//  void Core::f_createInstance(coreCreateInfo info){
//        
//
//        if (m_enableValidation && !checkValidationLayerSupport()) {
//            throw std::runtime_error("validation layers requested, but not available!");
//        }
//
//        VkApplicationInfo appInfo = {};
//        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
//        appInfo.pApplicationName = info.appName.c_str();
//        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
//        appInfo.pEngineName = "No Engine";
//        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
//        appInfo.apiVersion = VK_API_VERSION_1_0;
//
//        VkInstanceCreateInfo createInfo = {};
//        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
//        createInfo.pApplicationInfo = &appInfo;
//
//        auto extensions = f_getInstanceExtensions(info.requestedExtensions);
//        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
//        createInfo.ppEnabledExtensionNames = extensions.data();
//
//        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
//        if (m_enableValidation) {
//            std::cout<<"instance validation enabled"<<std::endl;
//            for( auto layer: validationLayers)
//            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
//            createInfo.ppEnabledLayerNames = validationLayers.data();
//
//            populateDebugMessengerCreateInfo(debugCreateInfo);
//            createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
//        } else {
//            createInfo.enabledLayerCount = 0;
//
//            createInfo.pNext = nullptr;
//      }
//
//      if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS) {
//          throw std::runtime_error("failed to create instance!");
//      }
//      std::cout<<"Instance created, party on ted"<<std::endl;
//
//
//
//      }
//
//
//      std::vector<const char*> Core::f_getInstanceExtensions(std::vector<const char*> requestedExtensions){
//       //uint32_t glfwExtensionCount = 0;
//       //   const char** glfwExtensions;
//       //   glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
//
//          std::vector<const char*> extensions(requestedExtensions.begin(),requestedExtensions.end());
//
//          if (m_enableValidation) {
//              extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
//          }
//          return extensions; 
//
//      }
//      void Core::f_setupDebugMessenging(){
//      
//          if (!m_enableValidation) return;
//          std::cout<<"validation ..."<<std::endl;
//          VkDebugUtilsMessengerCreateInfoEXT createInfo;
//          populateDebugMessengerCreateInfo(createInfo);
//          VkResult result = CreateDebugUtilsMessengerEXT(m_instance, &createInfo, nullptr, &m_debugMessenger);
//          if (result != VK_SUCCESS) {
//              std::cout<< result << std::endl;
//              throw std::runtime_error("failed to set up debug messenger!");
//          }
//          std::cout<<"ENABLED"<<endl;
//      
//      }
//
//      VkInstance Core::getInstance(){
//        return m_instance; 
//        }
//
//      void Core::registerDevice(Device D) {
//        m_device = D;
//      }
//
//      Device Core::getDevice(){
//        return m_device; 
//      }
//
}}

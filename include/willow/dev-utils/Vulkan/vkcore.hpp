#ifndef vkcore_h
#define vkcore_h
#include "vkinit.hpp"
#include "vktypes.hpp"
#include "vkdevice.hpp"
#include <cstdint>
#include<string>
#include<vector>
namespace wlo{
  namespace vk{
   
      VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);

      void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
      
  }
}
#endif

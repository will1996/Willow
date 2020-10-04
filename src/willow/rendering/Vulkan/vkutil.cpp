#include "willow/dev-utils/Vulkan/vkutil.hpp"
#include "willow/dev-utils/Vulkan/vktypes.hpp"
namespace wlo{
  namespace vk{
    void checkResult(VkResult res,std::string msg = "do something important") {
        std::string info = "\nError info: ";
        if(res == VK_SUCCESS) return;
        switch(res) {
          case VK_ERROR_INITIALIZATION_FAILED: 
             info+= "VK_ERROR_INITIALIZATION FAILED";
             break;
          case VK_ERROR_EXTENSION_NOT_PRESENT: 
             info+= "VK_ERROR_EXTENSION_NOT_PRESENT FAILED";
             break;
          default:
             break;
              
          
          
          
          }
        throw std::runtime_error("failed to"+msg+info);
    }
  
  
 QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device,VkSurfaceKHR surface){
        QueueFamilyIndices indices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        int i = 0;
        for (const auto& queueFamily : queueFamilies) {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.graphicsFamily = i;
            }

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

            if (presentSupport) {
                indices.presentFamily = i;
            }

            if (indices.isComplete()) {
                break;
            }

            i++;
        }

        return indices;
 
 }
  }



}
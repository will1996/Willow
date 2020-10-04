#ifndef vkinit_h
#define vkinit_h

#include<vulkan/vulkan.h>
#include<vector>
namespace wlo{
  namespace vk{

      const std::vector<const char*> validationLayers = {
          "VK_LAYER_KHRONOS_validation"
      };

      const std::vector<const char*> deviceExtensions = {
          VK_KHR_SWAPCHAIN_EXTENSION_NAME
      };

    namespace init{
         VkApplicationInfo appInfo();
         VkInstanceCreateInfo instanceInfo();
         VkImageCreateInfo imageInfo();
         VkBufferCreateInfo bufferInfo(); 
         VkSubmitInfo submitInfo();
         VkSamplerCreateInfo samplerInfo();
         VkPipelineLayoutCreateInfo pipelineLayoutInfo();
    }
  }
}//end namespace ::vk::init


#endif


#ifndef vkutil_h
#define vkutil_h
#include<vulkan/vulkan.hpp>
#include "vktypes.hpp"
#include<stdexcept>
#include<string>
namespace wlo{
  namespace wk{
    void checkResult(VkResult res,std::string);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice,VkSurfaceKHR);
  
  }


}
//run vk::checkResult IF we aren't debugging, in all other cases do nothing!
#ifndef NDEBUG
#define VK_CHECK(vkResult) wilo::vk::checkResult(vkResult)
#else
#define VK_CHECK(vkResult) vkResult
#endif

#endif

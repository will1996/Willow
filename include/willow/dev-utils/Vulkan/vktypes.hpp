#ifndef vktypes_h
#define vktypes_h
#include "vkinit.hpp"
#include <cstdint>
#include<optional>
#include<vector>
namespace wlo{
  namespace vk{
  
        struct SwapChainSupportDetails {
            VkSurfaceCapabilitiesKHR capabilities;
            std::vector<VkSurfaceFormatKHR> formats;
            std::vector<VkPresentModeKHR> presentModes;
        };
            struct QueueFamilyIndices{
            std::optional<uint32_t> graphicsFamily;
            std::optional<uint32_t> presentFamily;

            bool isComplete() {
                return graphicsFamily.has_value() && presentFamily.has_value();
            }
          };
  
  }

}
#endif

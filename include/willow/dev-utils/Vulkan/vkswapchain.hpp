#include "vkdevice.hpp"
namespace wlo{
  namespace wk{
    class Swapchain{
      public:
          Swapchain(Device,VkSurfaceKHR);
          ~Swapchain();
          VkExtent2D getExtent();
          uint32_t getNumImages();
          VkFormat getFormat(); 
          void initialize(uint32_t width, uint32_t height);
          void reclaim();
      private:
      Device m_device;
      VkSurfaceKHR m_surface;
      VkSwapchainKHR m_swapchain;
      std::vector<VkImage> m_swapchainImages;
      VkFormat m_swapchainImageFormat;
      VkExtent2D m_swapchainExtent;
      std::vector<VkImageView> m_swapchainImageViews;
      std::vector<VkFramebuffer> m_swapchainFramebuffers;
    }; 
  
  
  }
}

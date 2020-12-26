#ifndef vkdevice_h
#define vkdevice_h
#include "vktypes.hpp"
#include "vkutil.hpp"
#include <cstdint>
#include<optional>
#include<vector>
namespace wlo{
  namespace wk{
      
     class Device{
        //unifies VkPhysical devices and logical devices. 
        //Performs selection of physical device, 
        //and management of device queues
        //TODO make Devices capable of compute and memory management
        //
      public:
        
        Device();
        ~Device();
        void initialize(VkInstance,VkSurfaceKHR);
        void reclaim();
        VkDevice& logical();
        VkPhysicalDevice& physical();
        bool getValidationState();
        void setValidationState(bool set);
        VkQueue getPresentQueue();
        VkQueue getGraphicsQueue();
       private:
         VkDevice m_logicalDevice = VK_NULL_HANDLE;
         VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
         //an expandable vector of queues, mainly for future expandability
         std::vector<VkQueue> m_queues;
         //indices of the graphics and present queues within m_queues
         int m_graphicsQueueIndex = 0, m_presentQueueIndex = 1;

         //there are instance and device level validation layers, 
         //this toggle is set to a default value in the constructor based on 
         //compiler flags, but can be set with setValdidationState if you want
         bool m_enableValidation;
        std::vector<const char *> m_enabledLayers;
         std::vector<const char *> m_enabledExtensions;
         bool f_physicalDeviceSuitable(VkPhysicalDevice device,VkSurfaceKHR);
         void f_selectPhysicalDevice(VkInstance,VkSurfaceKHR);
         void f_createLogicalDevice(VkSurfaceKHR);
     
     } ;
  }
}
#endif

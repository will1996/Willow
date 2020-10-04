#pragma once
#include "willow/root/wilo_dev_core.hpp"
#include "willow/dev-utils/Vulkan/wilovk.hpp"
#include "willow/window/wilo_window.hpp"
#include <vulkan/vulkan.hpp>
namespace wlo{
            class VulkanRenderingContext {
                    //the vulkan rendering context spins up the Vulkan Basic environment. 
                    //the "context" owns the vulkan instance, physical and logical device handles, 
                    //the surface, and also the debug messenger

                public:

                    struct Info   {
                        bool enableValidationLayers;
                        
                    
                    };
                //creating a rendering context grants the context, and by extension the renderer
                //shared ownership of the window
                VulkanRenderingContext();
                void initialize(wlo::SharedPointer<Window> window, Info info) ;
                void reclaim() ;
                VkInstance getInstance() const;
                VkPhysicalDevice getPhysicalDevice()const ;
                VkDevice getDevice()const ;
                VkSurfaceKHR getSurface()const ;
                std::string getShadersPath()const ;
                vk::SwapChainSupportDetails getSwapChainSupportDetails() const ;
                ~VulkanRenderingContext();
                private:
                bool m_enableValidationLayers;
                void setupDebugging();//creates the debug messenger and sets the call back
			    std::vector<const char *> getInstanceExtensions();
                std::vector<const char *> getInstanceLayers();
                std::vector<const char*> getDeviceExtensions();
                void createInstance();//creates the vulkan instance, parsing the Application::info 
                void selectPhysicalDevice();//based on requirements specified by renderingcontextInfos a physical device is selected
                bool physicalDeviceIsSuitable(VkPhysicalDevice device);//returns true if the physical device in the argument meets the requirements. 
                void createLogicalDevice();//populates the physical and logical devices (first by validating everything)
                void createSurface();//creates a surface from the window that was passed in 
                wlo::SharedPointer<Window> p_window;
                VkInstance m_instance; 
                VkPhysicalDevice m_physicalDevice;
                VkDevice m_device; 
                VkSurfaceKHR m_surface;
                VkDebugUtilsMessengerEXT m_debugMessenger;
                VkQueue m_graphicsQueue;
                VkQueue m_presentQueue;
                std::string m_shadersPath;
            };

}
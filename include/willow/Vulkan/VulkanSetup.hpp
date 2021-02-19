//
// Created by W on 10/22/20.
//

#ifndef WILLOW_VULKANSETUP_HPP
#define WILLOW_VULKANSETUP_HPP
#include<vulkan/vulkan.hpp>
#include"willow/window/Window.hpp"
namespace wlo::wk{
   vk::Instance createInstance(std::vector<const char* > layerNames,std::vector<const char* > extensionNames,bool debugging);
   vk::DebugUtilsMessengerEXT createDebugMessenger(const vk::Instance &);
   vk::SurfaceKHR createSurface(const vk::Instance  &, SharedPointer<wlo::Window>  wind);
   vk::PhysicalDevice selectGPU(vk::Instance,VkSurfaceKHR);
   vk::Device createDevice(vk::Instance);

   vk::RenderPassCreateInfo createDefaultRenderPassInfo(vk::Format imageFormat);
}


#endif //WILLOW_VULKANSETUP_HPP

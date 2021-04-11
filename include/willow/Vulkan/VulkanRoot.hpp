//
// Created by W on 11/29/20.
//

#ifndef WILLOW_VULKANROOT_HPP
#define WILLOW_VULKANROOT_HPP
#include <vulkan/vulkan.hpp>
#include "willow/root/Root.hpp"
#include "willow/rendering/Renderer.hpp"
#include"willow/messaging/MessageSystem.hpp"
namespace wlo::wk {

    class VulkanRoot {
    public:
        VulkanRoot(std::initializer_list<wlo::rendering::Renderer::Features> features,Window&,bool debugUtils );
        bool supportSurface(vk::SurfaceKHR &surface);
        vk::Instance& Instance();
        vk::PhysicalDevice & PhysicalDevice();
        vk::Device & Device();

        uint32_t findMemoryType(vk::MemoryRequirements memRequirements, vk::MemoryPropertyFlags);
        auto & QueueFamilyIndices(){return m_queueFamilyIndices;}
        ~VulkanRoot();
    private:

        struct {
            uint32_t graphics;
            uint32_t present;
        } m_queueFamilyIndices;
        std::vector<const char *> getInstanceExtensions(std::initializer_list<wlo::rendering::Renderer::Features>);
        std::vector<const char *> getInstanceLayers(std::initializer_list<wlo::rendering::Renderer::Features>);
        std::vector<const char *> getDeviceExtensions(std::initializer_list<wlo::rendering::Renderer::Features>);

        std::map<wlo::rendering::Renderer::Features,std::vector<const char *> > m_layerLookup = {
                //{Renderer::Features::DebuggingTools, {"VK_LAYER_KHRONOS_validation"}}
        };
        std::map<wlo::rendering::Renderer::Features,std::vector<const char *> > m_extensionLookup = {
                // {Renderer::Features::DebuggingTools, {VK_EXT_DEBUG_UTILS_EXTENSION_NAME}}
        };
        std::map<wlo::rendering::Renderer::Features,std::vector<const char *> > m_deviceExtensionLookup = {
                // {Renderer::Features::DebuggingTools, {VK_EXT_DEBUG_UTILS_EXTENSION_NAME}}
        };
        void selectPhysicalDevice(vk::SurfaceKHR);
        void createLogicalDevice(std::initializer_list<wlo::rendering::Renderer::Features> features, vk::SurfaceKHR);
        //create a logical device for presenting to the provided surface type (usual behavior for a renderer)
        vk::DebugUtilsMessengerEXT m_debugMessenger;
        vk::Instance m_instance;
        vk::PhysicalDevice m_physicalDevice;
        vk::Device m_device;


    };
}


#endif //WILLOW_VULKANROOT_HPP

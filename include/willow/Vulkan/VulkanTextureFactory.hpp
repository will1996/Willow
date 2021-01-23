//
// Created by W on 1/18/21.
//

#ifndef WILLOW_VULKANTEXTUREFACTORY_HPP
#define WILLOW_VULKANTEXTUREFACTORY_HPP

#include"willow/Vulkan/VulkanRoot.hpp"
#include "willow/Vulkan/VulkanMemoryManager.hpp"
namespace wlo::wk {
    class VulkanTextureFactory {
    public:
        VulkanTextureFactory(VulkanRoot& root, VulkanMemoryManger & memoryManger);
        void createTexture2D(std::string imageFile);
        bool textureCreated(std::string imageFile);
        DeviceImage & fetchTexture(std::string imageFile);
    private:

        VulkanRoot& m_root;
        VulkanMemoryManger& m_memoryManager;
        std::unordered_map<std::string,DeviceImage> m_textures;

    };
}


#endif //WILLOW_VULKANTEXTUREFACTORY_HPP

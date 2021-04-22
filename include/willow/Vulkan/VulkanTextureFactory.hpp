//
// Created by W on 1/18/21.
//

#ifndef WILLOW_VULKANTEXTUREFACTORY_HPP
#define WILLOW_VULKANTEXTUREFACTORY_HPP

#include"willow/Vulkan/VulkanRoot.hpp"
#include "willow/Vulkan/VulkanMemoryManager.hpp"
#include "willow/rendering/Texture.hpp"
namespace wlo::wk {
    class VulkanTextureFactory {
    public:
        VulkanTextureFactory(VulkanRoot& root, VulkanMemoryManger & memoryManger);
        void bindHostTexture(const Texture& texture);
        bool textureCreated(wlo::ID_type);
        DeviceImage & fetchTexture(wlo::ID_type);
    private:

        VulkanRoot& m_root;
        VulkanMemoryManger& m_memoryManager;
        std::unordered_map<wlo::ID_type,DeviceImage> m_textures;

    };
}


#endif //WILLOW_VULKANTEXTUREFACTORY_HPP

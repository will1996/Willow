//
// Created by W on 1/18/21.
//

#include "willow/Vulkan/VulkanTextureFactory.hpp"
#include"willow/rendering/Texture.hpp"

namespace wlo::wk {
    wlo::wk::VulkanTextureFactory::VulkanTextureFactory(wlo::wk::VulkanRoot &root,
                                                        wlo::wk::VulkanMemoryManger &memoryManger)
                : m_root(root),
                m_memoryManager(memoryManger)
                {
                }

    void wlo::wk::VulkanTextureFactory::createTexture2D(std::string imageFile) {
        Texture tex(imageFile);

        //allocate mapped memory for the memory transfer
       MappedBuffer stagingBuffer = m_memoryManager.allocateMappedBuffer(tex.texelFormat(),
                                                                         tex.width()*tex.height(),
                                                                         vk::BufferUsageFlagBits::eTransferSrc);
        //move image memory to GPU
       memcpy(stagingBuffer.writePoint, tex.view().source, tex.view().memSize);

       DeviceImage image =  m_memoryManager.allocateImage(tex.texelFormat(),tex.width(),tex.height(),
                                      vk::ImageLayout::eTransferDstOptimal,
                                      vk::ImageUsageFlagBits::eTransferDst|vk::ImageUsageFlagBits::eSampled);
        m_memoryManager.copy(image,stagingBuffer);
        vk::ImageViewCreateInfo viewInfo(vk::ImageViewCreateFlags(), image.image.get(), vk::ImageViewType::e2D, vk::Format::eR8G8B8A8Srgb );
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.layerCount = 1;
        viewInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
        image.view = m_root.Device().createImageViewUnique(viewInfo);
        m_textures.insert({imageFile, std::move(image)});

    }

    bool VulkanTextureFactory::textureCreated(std::string imageFile) {
        return m_textures.contains(imageFile);
    }

    DeviceImage &VulkanTextureFactory::fetchTexture(std::string imageFile) {
        if(!textureCreated(imageFile))
            throw std::runtime_error("attempted to fetch texture: "+imageFile+"But this texture does not exist, you must first create the texture from the file");
        return m_textures[imageFile];
    }

}
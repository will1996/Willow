//
// Created by W on 1/18/21.
//

#include "willow/Vulkan/VulkanTextureFactory.hpp"
#define  STB_IMAGE_IMPLEMENTATION
#include"external/stb_image.h"
namespace wlo::wk {
    wlo::wk::VulkanTextureFactory::VulkanTextureFactory(wlo::wk::VulkanRoot &root,
                                                        wlo::wk::VulkanMemoryManger &memoryManger)
                : m_root(root),
                m_memoryManager(memoryManger)
                {
                }

    void wlo::wk::VulkanTextureFactory::createTexture2D(std::string imageFile) {
        int texWidth,texHeight,texChannels;
        stbi_uc* pixels = stbi_load(imageFile.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
        WILO_CORE_INFO("loading texture, WARNING texture must have rgba format");

        if(pixels==nullptr){
            throw std::runtime_error("Failed to load image from file: "+imageFile);
        }

        wlo::rendering::DataLayout pixelLayout{
                {wlo::rendering::DataLayout::DataType::Byte,4}
        };
        //allocate mapped memory for the memory transfer
       MappedBuffer stagingBuffer = m_memoryManager.allocateMappedBuffer(pixelLayout,
                                                                          texWidth*texHeight,
                                                                         vk::BufferUsageFlagBits::eTransferSrc);
        //move image memory to GPU
       memcpy(stagingBuffer.writePoint, pixels, pixelLayout.memSize() * texWidth * texHeight);
        //free the host copy of the image data
       stbi_image_free(pixels);

       DeviceImage image =  m_memoryManager.allocateImage(pixelLayout,texWidth,texHeight,
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
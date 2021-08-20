//
// Created by W on 1/18/21.
//

#include <willow/Vulkan/VulkanMemoryManager.hpp>

namespace wlo::wk {
    VulkanMemoryManger::VulkanMemoryManger(VulkanRoot &root,VulkanCommandInterface & commandInterface):
    m_root(root),
    m_commandInterface(commandInterface),
    m_totalAllocationSize(0)
    {

    }
    MappedBuffer wlo::wk::VulkanMemoryManger::allocateMappedBuffer(const wlo::data::Type &layout,
                                                                   size_t count,
                                                                   vk::BufferUsageFlagBits usage)
       {
           wk::MappedBuffer buff{

                  {allocateBuffer(layout, count, usage,
                                  vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent)},
                                  /*.writePointr =*/ nullptr
           };


        buff.writePoint = (byte *) m_root.Device().mapMemory(buff.memory.get(), 0, layout.footprint() * count);

        memset(buff.writePoint, 0, count*layout.footprint());

        m_root.Device().bindBufferMemory(*buff.buffer, *buff.memory, 0);
        return buff;
    }

    DeviceBuffer VulkanMemoryManger::allocateBuffer(const wlo::data::Type &layout, size_t count,
                                                    vk::BufferUsageFlagBits usage,
                                                    vk::MemoryPropertyFlags memproperties
                                                    ) {
        DeviceBuffer buff;
        vk::BufferCreateInfo buffInfo(vk::BufferCreateFlags(), count * layout.footprint(), usage);
        buff.buffer = m_root.Device().createBufferUnique(buffInfo);
        vk::MemoryRequirements memoryRequirements = m_root.Device().getBufferMemoryRequirements(buff.buffer.get());

        uint32_t vertexMemoryTypeIndex = m_root.findMemoryType(memoryRequirements, memproperties);

        vk::MemoryAllocateInfo allocInfo(memoryRequirements.size, vertexMemoryTypeIndex);
        buff.memory = m_root.Device().allocateMemoryUnique(allocInfo);
        m_totalAllocationSize+=memoryRequirements.size;
        return buff;
    }
    void setImageLayout( vk::UniqueCommandBuffer const & commandBuffer,
                         vk::Image                       image,
                         vk::Format                      format,
                         vk::ImageLayout                 oldImageLayout,
                         vk::ImageLayout                 newImageLayout )
    {
        vk::AccessFlags sourceAccessMask;
        switch ( oldImageLayout )
        {
            case vk::ImageLayout::eTransferDstOptimal: sourceAccessMask = vk::AccessFlagBits::eTransferWrite; break;
            case vk::ImageLayout::ePreinitialized: sourceAccessMask = vk::AccessFlagBits::eHostWrite; break;
            case vk::ImageLayout::eGeneral:  // sourceAccessMask is empty
            case vk::ImageLayout::eUndefined: break;
            default: assert( false ); break;
        }

        vk::PipelineStageFlags sourceStage;
        switch ( oldImageLayout )
        {
            case vk::ImageLayout::eGeneral:
            case vk::ImageLayout::ePreinitialized: sourceStage = vk::PipelineStageFlagBits::eHost; break;
            case vk::ImageLayout::eTransferDstOptimal: sourceStage = vk::PipelineStageFlagBits::eTransfer; break;
            case vk::ImageLayout::eUndefined: sourceStage = vk::PipelineStageFlagBits::eTopOfPipe; break;
            default: assert( false ); break;
        }

        vk::AccessFlags destinationAccessMask;
        switch ( newImageLayout )
        {
            case vk::ImageLayout::eColorAttachmentOptimal:
                destinationAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
                break;
            case vk::ImageLayout::eDepthStencilAttachmentOptimal:
                destinationAccessMask =
                        vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite;
                break;
            case vk::ImageLayout::eGeneral:  // empty destinationAccessMask
            case vk::ImageLayout::ePresentSrcKHR: break;
            case vk::ImageLayout::eShaderReadOnlyOptimal: destinationAccessMask = vk::AccessFlagBits::eShaderRead; break;
            case vk::ImageLayout::eTransferSrcOptimal: destinationAccessMask = vk::AccessFlagBits::eTransferRead; break;
            case vk::ImageLayout::eTransferDstOptimal: destinationAccessMask = vk::AccessFlagBits::eTransferWrite; break;
            default: assert( false ); break;
        }

        vk::PipelineStageFlags destinationStage;
        switch ( newImageLayout )
        {
            case vk::ImageLayout::eColorAttachmentOptimal:
                destinationStage = vk::PipelineStageFlagBits::eColorAttachmentOutput;
                break;
            case vk::ImageLayout::eDepthStencilAttachmentOptimal:
                destinationStage = vk::PipelineStageFlagBits::eEarlyFragmentTests;
                break;
            case vk::ImageLayout::eGeneral: destinationStage = vk::PipelineStageFlagBits::eHost; break;
            case vk::ImageLayout::ePresentSrcKHR: destinationStage = vk::PipelineStageFlagBits::eBottomOfPipe; break;
            case vk::ImageLayout::eShaderReadOnlyOptimal:
                destinationStage = vk::PipelineStageFlagBits::eFragmentShader;
                break;
            case vk::ImageLayout::eTransferDstOptimal:
            case vk::ImageLayout::eTransferSrcOptimal: destinationStage = vk::PipelineStageFlagBits::eTransfer; break;
            default: assert( false ); break;
        }

        vk::ImageAspectFlags aspectMask;
        if ( newImageLayout == vk::ImageLayout::eDepthStencilAttachmentOptimal )
        {
            aspectMask = vk::ImageAspectFlagBits::eDepth;
            if ( format == vk::Format::eD32SfloatS8Uint || format == vk::Format::eD24UnormS8Uint )
            {
                aspectMask |= vk::ImageAspectFlagBits::eStencil;
            }
        }
        else
        {
            aspectMask = vk::ImageAspectFlagBits::eColor;
        }

        vk::ImageSubresourceRange imageSubresourceRange( aspectMask, 0, 1, 0, 1 );
        vk::ImageMemoryBarrier    imageMemoryBarrier( sourceAccessMask,
                                                      destinationAccessMask,
                                                      oldImageLayout,
                                                      newImageLayout,
                                                      VK_QUEUE_FAMILY_IGNORED,
                                                      VK_QUEUE_FAMILY_IGNORED,
                                                      image,
                                                      imageSubresourceRange );
        return commandBuffer->pipelineBarrier( sourceStage, destinationStage, {}, nullptr, nullptr, imageMemoryBarrier );
    }

    void VulkanMemoryManger::copy(DeviceImage &to, DeviceBuffer &from) {
        //transition memory layout of the image
        m_commandInterface.submitImmediately([&](vk::UniqueCommandBuffer& cmd){
                vk::ImageSubresourceRange resourceRange(vk::ImageAspectFlagBits::eColor,0,1,0,1);
                //transition layout to writable from the host
                vk::ImageMemoryBarrier writeableTransition({},
                                                           vk::AccessFlagBits::eTransferWrite,
                                                           vk::ImageLayout::eUndefined,
                                                           vk::ImageLayout::eTransferDstOptimal);
                writeableTransition.image = to.image.get();
                writeableTransition.subresourceRange = resourceRange;
                cmd->pipelineBarrier(vk::PipelineStageFlagBits::eTopOfPipe, vk::PipelineStageFlagBits::eTransfer, {}, nullptr, nullptr, writeableTransition);

                //copy data from host to image
                vk::BufferImageCopy copyDesc;
                copyDesc.imageSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
                copyDesc.imageSubresource.layerCount = 1;
                copyDesc.imageExtent = to.imageExtent;
                cmd->copyBufferToImage(from.buffer.get(),to.image.get(),vk::ImageLayout::eTransferDstOptimal,copyDesc);

                //transntion to readable by the shaders
                vk::ImageMemoryBarrier readableTransition(vk::AccessFlagBits::eTransferWrite,
                                                           vk::AccessFlagBits::eShaderRead,
                                                           vk::ImageLayout::eTransferDstOptimal,
                                                           vk::ImageLayout::eShaderReadOnlyOptimal);
                readableTransition.image = to.image.get();
                readableTransition.subresourceRange = resourceRange;
                cmd->pipelineBarrier(vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eFragmentShader, {}, nullptr, nullptr, readableTransition);
                to.layout = vk::ImageLayout::eShaderReadOnlyOptimal;
        });

    }

    vk::Format fromLayout(wlo::data::Type t) {
       if(t==data::typeOf<Pixel4>())
          return vk::Format::eR8G8B8A8Srgb;
       else  if(t==data::typeOf<Pixel3>())
           return vk::Format::eR8G8B8Srgb;
       else  if(t==data::typeOf<Pixel2>())
           return vk::Format::eR8G8Srgb;
       else  if(t==data::typeOf<Pixel1>())
           return vk::Format::eR8Srgb;
       else
           throw std::runtime_error("Unsupported image format, only supporting 8 bit Pixel data, 1-4 channel");
    }

    DeviceImage VulkanMemoryManger::allocateImage(const wlo::data::Type &pixelLayout,
                                                  uint32_t width ,
                                                  uint32_t height,
                                                  vk::ImageLayout imageLayout,
                                                  vk::ImageUsageFlags usage)
    {
        vk::Extent3D imageExtent;
        imageExtent.height = height;
        imageExtent.width = width;
        imageExtent.depth = 1;

       // if(pixelLayout!=data::typeOf<Pixel4>())
        //    throw std::runtime_error("image format unsupported, currently only supporting rbga");
        vk::Format image_format = fromLayout(pixelLayout);

        vk::ImageCreateInfo imageCreateInfo(vk::ImageCreateFlags(),vk::ImageType::e2D,image_format,imageExtent);
        imageCreateInfo.usage = usage;
        imageCreateInfo.mipLevels = 1;
        imageCreateInfo.arrayLayers = 1;

        DeviceImage image;
        image.imageExtent = imageExtent;
        image.image = m_root.Device().createImageUnique(imageCreateInfo);
        image.layout = imageLayout;

        vk::MemoryRequirements memoryRequirements = m_root.Device().getImageMemoryRequirements(image.image.get());
        auto typeIndex  = m_root.findMemoryType(memoryRequirements,vk::MemoryPropertyFlagBits::eDeviceLocal);
        vk::MemoryAllocateInfo imageAllocateInfo(memoryRequirements.size,typeIndex);

        image.memory = m_root.Device().allocateMemoryUnique(imageAllocateInfo);

        m_root.Device().bindImageMemory(image.image.get(),image.memory.get(),0);

        m_totalAllocationSize+=memoryRequirements.size;
        return image;
    }


        size_t VulkanMemoryManger::getTotalAllocationSize(){
            return m_totalAllocationSize;
        }
}

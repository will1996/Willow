#include<vulkan/vulkan.h>
#include "willow/dev-utils/Vulkan/vkinit.hpp"

namespace wlo{
  namespace vk{
    namespace init{

VkApplicationInfo appInfo(){
  VkApplicationInfo appInfo = {};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  return appInfo;
 }

VkInstanceCreateInfo instanceInfo(){
  VkInstanceCreateInfo instanceInfo = {};
  instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  return instanceInfo;
 }

VkImageCreateInfo imageInfo(){
  VkImageCreateInfo imageInfo = {};
  imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  return imageInfo;
 }

 VkBufferCreateInfo bufferInfo(){
  VkBufferCreateInfo bufferInfo = {};
  bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  return bufferInfo;
  }

 VkSubmitInfo submitInfo(){
  VkSubmitInfo submitInfo = {};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  return submitInfo;
  }

 VkSamplerCreateInfo samplerInfo(){
  VkSamplerCreateInfo samplerInfo = {};
  samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
  return samplerInfo;
  }

 VkPipelineLayoutCreateInfo pipelineLayoutInfo(){
  VkPipelineLayoutCreateInfo pipelineInfo = {};
  pipelineInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  return pipelineInfo;
 }

    }
  }
}

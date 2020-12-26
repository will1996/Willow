#include "willow/rendering/Vulkan/vulkan_shader.hpp"
#include <fstream>

//this is a comment
namespace wlo{
    VulkanShader::VulkanShader(VkDevice device, std::string shaderFullPath):
            wlo::Shader(shaderFullPath)
    {
        WILO_CORE_INFO("creating a vulkan shader from the file at:{0} ", shaderFullPath);

        const std::vector<char> &byte_code = getCode();
            VkShaderModuleCreateInfo moduleInfo{};
            moduleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
            moduleInfo.codeSize = byte_code.size();
            moduleInfo.pCode = reinterpret_cast<const uint32_t *> (byte_code.data());//hoo boy

            VkResult res = vkCreateShaderModule(device,&moduleInfo,nullptr,&m_module);

            vk::checkResult(res,"create Shader Module");

            VkPipelineShaderStageCreateInfo stageInfo{};

            stageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            
            if(getType() == wlo::Shader::Type::Vertex)
             stageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
            else if (getType() == wlo::Shader::Type::Fragment)
             stageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
            else
            WILO_ASSERT(false);


             stageInfo.pName = "main";
             stageInfo.module = m_module;

            m_info = stageInfo; 
    }
    VulkanShader::~VulkanShader(){
    
    }
    //TODO make shaders compiled from source code, rather than read in as binaries


    VkShaderModule VulkanShader::getModule() const {
        return m_module;
    }


    VkPipelineShaderStageCreateInfo VulkanShader::getInfo() const{
        return m_info;
    }

   const wlo::Shader::Type VulkanShader::getType() const {
       return wlo::Shader::getType();
   }

   void VulkanShader::reclaim(VkDevice device)
   {
           vkDestroyShaderModule(device, m_module,nullptr);
   }



}
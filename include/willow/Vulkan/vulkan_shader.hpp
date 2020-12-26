#pragma once
#include"willow/rendering/Shader.hpp"
#include"VulkanSwapchain.hpp"
#include "vulkan_context.hpp"


namespace wlo{
    class VulkanShader : public wlo::Shader{
        public:
            VulkanShader(VkDevice device, std::string shaderFile);
        
            //virtual void compile() override;
         
        VkShaderModule getModule() const;
        VkPipelineShaderStageCreateInfo getInfo() const;
        const wlo::Shader::Type getType() const override;

        void reclaim(VkDevice device);
        
        ~VulkanShader();
        private:
        
        VkShaderModule m_module;
        VkPipelineShaderStageCreateInfo m_info;


         
        
        };

}

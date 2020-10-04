#pragma once
#include"willow/rendering/wilo_shader.hpp"
#include"vulkan_swapchain.hpp"
#include "vulkan_context.hpp"


namespace wlo{
    class VulkanShader : public wlo::Shader{
        public:
            VulkanShader(VkDevice device, std::string shaderFile);
        
            virtual void compile() override;
         
        VkShaderModule getModule() const;
        VkPipelineShaderStageCreateInfo getInfo() const;
        wlo::Shader::Type getType() const;

        void reclaim(VkDevice device);
        
        ~VulkanShader();
        private:
        
        VkShaderModule m_module;
        VkPipelineShaderStageCreateInfo m_info;


         
        
        };

}

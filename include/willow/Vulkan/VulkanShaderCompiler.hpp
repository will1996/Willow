//
// Created by W on 12/30/20.
//

#ifndef WILLOW_VULKANSHADERCOMPILER_HPP
#define WILLOW_VULKANSHADERCOMPILER_HPP
#include"willow/root/wilo_dev_core.hpp"
#include"VulkanRoot.hpp"
namespace wlo::wk {
    class VulkanShader {
        VulkanShader(std::string filepath);
        vk::ShaderModule m_module;
        vk::PipelineShaderStageCreateInfo m_stageInfo;
        wlo::rendering::DataLayout m_vertexInputLayout;
        wlo::rendering::DataLayout m_uniformBufferLayout;
        wlo::rendering::DataLayout m_pushConstantLayout;
        friend class VulkanShaderCompiler;
        friend class std::map<std::string, VulkanShader>;
    public:
        VulkanShader() = default;
        const std::string path;
        vk::PipelineShaderStageCreateInfo& pipelineStageInfo();
        wlo::rendering::DataLayout& vertexInputLayout();
        wlo::rendering::DataLayout& pushConstantLayout();
        wlo::rendering::DataLayout& uniformBufferLayout();
    };
    
    class VulkanShaderCompiler {
    public:
        VulkanShaderCompiler(VulkanRoot &);
        VulkanShader& ShaderFromBinary(vk::ShaderStageFlagBits, const std::string& filepath);
        VulkanShader& fetchShader(const std::string& filepath);
        
        ~VulkanShaderCompiler();
  vk::UniqueShaderModule makeModuleFromBinary(vk::ShaderStageFlagBits,const std::string & filepath);

    private:
        VulkanRoot& m_root;
        std::unordered_map<std::string, VulkanShader> m_shaders;
        std::vector<uint32_t> readSpirvBinary(const std::string& filepath);
        wlo::rendering::DataLayout getVertexLayout(const std::vector<uint32_t>& code);
        wlo::rendering::DataLayout getUniformLayout(const std::vector<uint32_t>& code);
        wlo::rendering::DataLayout getPushConstant(const std::vector<uint32_t>& code);
    };

}

#endif //WILLOW_VULKANSHADERCOMPILER_HPP

//
// Created by W on 12/30/20.
//

#ifndef WILLOW_VULKANSHADERCOMPILER_HPP
#define WILLOW_VULKANSHADERCOMPILER_HPP
#include"willow/root/Root.hpp"
#include"VulkanRoot.hpp"

namespace wlo::wk {
    class VulkanShader {
        VulkanShader(std::string filepath);
        vk::ShaderModule m_module;
        vk::PipelineShaderStageCreateInfo m_stageInfo;
        std::vector<wlo::data::Type> m_vertexInputLayout;
        std::vector<wlo::data::Type> m_uniformBufferLayout;
        std::vector<wlo::data::Type> m_pushConstantLayout;
        friend class VulkanShaderCompiler;
        friend class std::map<std::string, VulkanShader>;
    public:
        VulkanShader() = default;
        const std::string path;
        vk::PipelineShaderStageCreateInfo& pipelineStageInfo();
        std::vector<wlo::data::Type>& vertexInputLayout();
        std::vector<wlo::data::Type>& pushConstantLayout();
        std::vector<wlo::data::Type>& uniformBufferLayout();
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

        std::vector<wlo::data::Type> getVertexLayout(const std::vector<uint32_t>& code);
        std::vector<wlo::data::Type> getUniformLayout(const std::vector<uint32_t>& code);
        std::vector<wlo::data::Type> getPushConstant(const std::vector<uint32_t>& code);
    };

}

#endif //WILLOW_VULKANSHADERCOMPILER_HPP

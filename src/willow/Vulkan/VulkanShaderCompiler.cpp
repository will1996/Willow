//
// Created by W on 12/30/20.
//

#include "willow/Vulkan/VulkanShaderCompiler.hpp"
#include"glslang/Include/ResourceLimits.h"
#include "spirv_cross/spirv_cross.hpp"
#include "willow/root/wilo_dev_core.hpp"

#include<fstream>

using namespace spirv_cross;
using namespace wlo::rendering;
namespace wlo::wk {
    wlo::wk::VulkanShader::VulkanShader(std::string filepath) : path(filepath) {}

    vk::PipelineShaderStageCreateInfo &wlo::wk::VulkanShader::pipelineStageInfo()  {
        return m_stageInfo;
    }

    wlo::rendering::DataLayout &VulkanShader::vertexInputLayout() {
        return m_vertexInputLayout;
    }

    wlo::rendering::DataLayout &VulkanShader::pushConstantLayout() {
        return m_pushConstantLayout;
    }

    wlo::rendering::DataLayout &VulkanShader::uniformBufferLayout() {
        return m_uniformBufferLayout;
    }



    wlo::wk::VulkanShaderCompiler::VulkanShaderCompiler(VulkanRoot &root) : m_root(root) {
        WILO_CORE_INFO("Vulkan shader compiler online");
    }


    wlo::wk::VulkanShader &
    wlo::wk::VulkanShaderCompiler::ShaderFromBinary(vk::ShaderStageFlagBits stageBits, const std::string &filepath) {
        VulkanShader shader(filepath);

        auto shaderCode = readSpirvBinary(filepath);

        shader.m_module = m_root.Device().createShaderModule(
                vk::ShaderModuleCreateInfo{vk::ShaderModuleCreateFlags(), shaderCode});

        shader.m_stageInfo = vk::PipelineShaderStageCreateInfo{vk::PipelineShaderStageCreateFlagBits(),
                                                               stageBits,shader.m_module,
                                                               "main"};

        shader.m_vertexInputLayout = getVertexLayout(shaderCode);
        shader.m_uniformBufferLayout = getUniformLayout(shaderCode);
        shader.m_pushConstantLayout = getPushConstant(shaderCode);
        m_shaders.insert({filepath,shader});

        return m_shaders[filepath];
    }

    VulkanShaderCompiler::~VulkanShaderCompiler() {
        for(auto & [_,shader] : m_shaders){
            m_root.Device().destroy(shader.m_module);
        }
        WILO_CORE_INFO("Vulkan shader compiler shutdown successfully");
    }


    std::vector<uint32_t> VulkanShaderCompiler::readSpirvBinary(const std::string &filepath) {
        std::ifstream file(filepath, std::ios::ate | std::ios::binary);
        size_t fileSize = file.tellg();
        file.seekg(0);
        if (fileSize % 4 != 0)
            throw std::runtime_error("FAILED TO CREATE SHADER THE FILE WAS NOT A SPIRV BINARY");

        vector<uint32_t> code(fileSize / 4);
        file.read((char *) code.data(), code.size() * 4);

        return code;
    }

    wlo::rendering::DataLayout VulkanShaderCompiler::getVertexLayout(const std::vector<uint32_t> &code) {
        spirv_cross::Compiler comp(code.data(), code.size());
        spirv_cross::ShaderResources shaderResources = comp.get_shader_resources();
        std::vector<DataLayout::Element> vertexInputElements(shaderResources.stage_inputs.size());
        for (auto &vertexInput: shaderResources.stage_inputs) {
            SPIRType type = comp.get_type(vertexInput.base_type_id);
            auto attachmentIndex = comp.get_decoration(vertexInput.id,spv::DecorationLocation);
            cout<<attachmentIndex<<endl;
            cout << "VertexInput found, it is an array with  " << type.vecsize << " elements" << endl;
            switch (type.basetype) {
                case SPIRType::BaseType::Float:
                    vertexInputElements[attachmentIndex] = {DataLayout::DataType::Float, type.vecsize};
                    break;
                case SPIRType::BaseType::Int:
                    vertexInputElements[attachmentIndex] = {DataLayout::DataType::Int, type.vecsize};
                    break;
                case SPIRType::BaseType::Boolean:
                    vertexInputElements[attachmentIndex] = {DataLayout::DataType::Bool, type.vecsize};
                    break;
            }

        }
        return wlo::rendering::DataLayout(vertexInputElements);
    }

    wlo::rendering::DataLayout VulkanShaderCompiler::getUniformLayout(const std::vector<uint32_t> &code) {
        spirv_cross::Compiler comp(code.data(), code.size());
        spirv_cross::ShaderResources shaderResources = comp.get_shader_resources();

        std::vector<DataLayout::Element> uniformInputElements;
        for (auto &uniform : shaderResources.uniform_buffers) {
            uint32_t set = comp.get_decoration(uniform.id, spv::DecorationDescriptorSet);
            uint32_t binding = comp.get_decoration(uniform.id, spv::DecorationBinding);
            const SPIRType type = comp.get_type(uniform.base_type_id);
            for(auto memberTypeID : type.member_types) {
                auto memberType = comp.get_type(memberTypeID);
                switch (memberType.basetype) {
                    case SPIRType::BaseType::Float:
                        uniformInputElements.push_back({DataLayout::DataType::Float, memberType.vecsize*memberType.columns});
                        break;
                }
            }
        }
        return wlo::rendering::DataLayout(uniformInputElements);
    }

    wlo::rendering::DataLayout VulkanShaderCompiler::getPushConstant(const std::vector<uint32_t> &code) {
        spirv_cross::Compiler comp(code.data(), code.size());
        spirv_cross::ShaderResources shaderResources = comp.get_shader_resources();

        std::vector<DataLayout::Element> pushConstantElements;
        for (auto &pushBuffer : shaderResources.push_constant_buffers) {
            uint32_t set = comp.get_decoration(pushBuffer.id, spv::DecorationDescriptorSet);
            uint32_t binding = comp.get_decoration(pushBuffer.id, spv::DecorationBinding);
            const SPIRType type = comp.get_type(pushBuffer.base_type_id);
            for(auto memberTypeID : type.member_types) {
                auto memberType = comp.get_type(memberTypeID);
                switch (memberType.basetype) {
                    case SPIRType::BaseType::Float:
                        pushConstantElements.push_back({DataLayout::DataType::Float, memberType.vecsize*memberType.columns});
                        break;
                }
            }

        }

        return wlo::rendering::DataLayout(pushConstantElements);
    }

    VulkanShader &VulkanShaderCompiler::fetchShader(const std::string &filepath) {
        return m_shaders[filepath];
    }
}
//
// Created by W on 12/30/20.
//

#include "willow/Vulkan/VulkanShaderCompiler.hpp"
#include"glslang/Include/ResourceLimits.h"
#include "spirv_cross/spirv_cross.hpp"
#include "willow/root/Root.hpp"

#include<fstream>

using namespace spirv_cross;
using namespace wlo::rendering;
namespace wlo::wk {
    wlo::wk::VulkanShader::VulkanShader(std::string filepath) : path(filepath) {}

    vk::PipelineShaderStageCreateInfo &wlo::wk::VulkanShader::pipelineStageInfo()  {
        return m_stageInfo;
    }

    std::vector<wlo::data::Type> &VulkanShader::vertexInputLayout() {
        return m_vertexInputLayout;
    }

    std::vector<wlo::data::Type> &VulkanShader::pushConstantLayout() {
        return m_pushConstantLayout;
    }

    std::vector<wlo::data::Type> &VulkanShader::uniformBufferLayout() {
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
            throw std::runtime_error("FAILED TO CREATE SHADER THE FILE  AT "+filepath+" WAS NOT A SPIRV BINARY");

        vector<uint32_t> code(fileSize / 4);
        file.read((char *) code.data(), code.size() * 4);

        return code;
    }

    std::vector<wlo::data::Type> VulkanShaderCompiler::getVertexLayout(const std::vector<uint32_t> &code) {
        spirv_cross::Compiler comp(code.data(), code.size());
        spirv_cross::ShaderResources shaderResources = comp.get_shader_resources();
        vector<wlo::data::Type> layoutMembers(shaderResources.stage_inputs.size());
        for (auto &vertexInput: shaderResources.stage_inputs) {
            SPIRType type = comp.get_type(vertexInput.base_type_id);
            auto attachmentIndex = comp.get_decoration(vertexInput.id, spv::DecorationLocation);
            cout << attachmentIndex << endl;

            cout << "VertexInput found, it is an array with  " << type.vecsize << " elements" << endl;
            if (type.basetype == SPIRType::BaseType::Float) {
                if (type.vecsize == 2)
                    layoutMembers[attachmentIndex] = wlo::data::Type::of<wlo::Vec2>();
                else if (type.vecsize == 3)
                    layoutMembers[attachmentIndex] = wlo::data::Type::of<wlo::Vec3>();
                else if(type.vecsize==4)
                    layoutMembers[attachmentIndex] = wlo::data::Type::of<wlo::Vec4>();
            }
            else
                throw std::runtime_error("unsupported type from shader");
        }
        return layoutMembers;
    }


    std::vector<wlo::data::Type> VulkanShaderCompiler::getUniformLayout(const std::vector<uint32_t> &code) {
        spirv_cross::Compiler comp(code.data(), code.size());
        spirv_cross::ShaderResources shaderResources = comp.get_shader_resources();
        vector<wlo::data::Type> layoutMembers(shaderResources.stage_inputs.size());
        for (auto &uniform : shaderResources.uniform_buffers) {
            //          uint32_t set = comp.get_decoration(uniform.id, spv::DecorationDescriptorSet);
            //         uint32_t binding = comp.get_decoration(uniform.id, spv::DecorationBinding);
            const SPIRType type = comp.get_type(uniform.base_type_id);
            for (auto memberTypeID : type.member_types) {
                auto memberType = comp.get_type(memberTypeID);
                if (memberType.basetype == SPIRType::BaseType::Float) {
                    size_t floatCount = memberType.vecsize*memberType.columns;
                    if (floatCount == 3)
                        layoutMembers.emplace_back(wlo::data::Type::of<Vec3>());
                    else if (floatCount == 4)
                        layoutMembers.emplace_back(wlo::data::Type::of<Vec4>());
                    else if (floatCount == 9)
                        layoutMembers.emplace_back(wlo::data::Type::of<Mat3>());
                    else if (floatCount == 16)
                        layoutMembers.emplace_back(wlo::data::Type::of<Mat4>());
                } else
                    throw std::runtime_error("unsupported type from shader");
            }
        }
        return layoutMembers;
    }
    std::vector<wlo::data::Type> VulkanShaderCompiler::getPushConstant(const std::vector<uint32_t> &code) {
        spirv_cross::Compiler comp(code.data(), code.size());
        spirv_cross::ShaderResources shaderResources = comp.get_shader_resources();

        vector<wlo::data::Type> layoutMembers;
        for (auto &pushBuffer : shaderResources.push_constant_buffers) {
//            uint32_t set = comp.get_decoration(pushBuffer.id, spv::DecorationDescriptorSet);
            //           uint32_t binding = comp.get_decoration(pushBuffer.id, spv::DecorationBinding);
            const SPIRType type = comp.get_type(pushBuffer.base_type_id);
            for (auto memberTypeID : type.member_types) {
                auto memberType = comp.get_type(memberTypeID);

                if(memberType.basetype==SPIRType::Float){
                size_t floatCount = memberType.vecsize*memberType.columns;
                if (floatCount == 3)
                    layoutMembers.emplace_back(wlo::data::Type::of<Vec3>());
                else if (floatCount == 4)
                    layoutMembers.emplace_back(wlo::data::Type::of<Vec4>());
                else if (floatCount == 9)
                    layoutMembers.emplace_back(wlo::data::Type::of<Mat3>());
                else if (floatCount == 16)
                    layoutMembers.emplace_back(wlo::data::Type::of<Mat4>());
                 }
                else
                    throw std::runtime_error("unsupported type from shader");

            }
        }
        return layoutMembers;
    }

    VulkanShader &VulkanShaderCompiler::fetchShader(const std::string &filepath) {
        return m_shaders[filepath];
    }
}

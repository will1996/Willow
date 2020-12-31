//
// Created by W on 12/30/20.
//

#include "willow/Vulkan/VulkanShaderCompiler.hpp"
#include"glslang/Include/ResourceLimits.h"
#include"SPIRV/GlslangToSpv.h"
#include "willow/root/wilo_dev_core.hpp"

#include<fstream>
wlo::wk::VulkanShaderCompiler::VulkanShaderCompiler(VulkanRoot & root):m_root(root) {
//    glslang::InitializeProcess();// start up the shader compiler
    WILO_CORE_INFO("Vulkan shader compiler online");

}

EShLanguage translateShaderStage( vk::ShaderStageFlagBits stage )
{
    switch ( stage )
    {
        case vk::ShaderStageFlagBits::eVertex: return EShLangVertex;
        case vk::ShaderStageFlagBits::eTessellationControl: return EShLangTessControl;
        case vk::ShaderStageFlagBits::eTessellationEvaluation: return EShLangTessEvaluation;
        case vk::ShaderStageFlagBits::eGeometry: return EShLangGeometry;
        case vk::ShaderStageFlagBits::eFragment: return EShLangFragment;
        case vk::ShaderStageFlagBits::eCompute: return EShLangCompute;
        case vk::ShaderStageFlagBits::eRaygenNV: return EShLangRayGenNV;
        case vk::ShaderStageFlagBits::eAnyHitNV: return EShLangAnyHitNV;
        case vk::ShaderStageFlagBits::eClosestHitNV: return EShLangClosestHitNV;
        case vk::ShaderStageFlagBits::eMissNV: return EShLangMissNV;
        case vk::ShaderStageFlagBits::eIntersectionNV: return EShLangIntersectNV;
        case vk::ShaderStageFlagBits::eCallableNV: return EShLangCallableNV;
        case vk::ShaderStageFlagBits::eTaskNV: return EShLangTaskNV;
        case vk::ShaderStageFlagBits::eMeshNV: return EShLangMeshNV;
        default: assert( false && "Unknown shader stage" ); return EShLangVertex;
    }
}

extern TBuiltInResource DefaultTBuiltInResource;
 vk::UniqueShaderModule wlo::wk::VulkanShaderCompiler::makeModuleFromBinary(vk::ShaderStageFlagBits stageBits,const std::string & filepath) {
    std::ifstream file(filepath, std::ios::ate | std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
    }
    size_t fileSizeBytes = file.tellg();
    assert(fileSizeBytes%4==0);

    std::vector<uint32_t> shaderCode(fileSizeBytes/4);
    file.seekg(0) ;
    file.read((char*) shaderCode.data(),fileSizeBytes);
    return m_root.Device().createShaderModuleUnique(vk::ShaderModuleCreateInfo{vk::ShaderModuleCreateFlags(), shaderCode });
}

wlo::wk::VulkanShaderCompiler::~VulkanShaderCompiler() {
//    glslang::FinalizeProcess();
    WILO_CORE_INFO("Vulkan shader compiler shutdown successfully");
}

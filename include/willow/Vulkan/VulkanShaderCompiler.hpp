//
// Created by W on 12/30/20.
//

#ifndef WILLOW_VULKANSHADERCOMPILER_HPP
#define WILLOW_VULKANSHADERCOMPILER_HPP
#include"willow/root/wilo_dev_core.hpp"
#include"VulkanRoot.hpp"
namespace wlo::wk {
    class VulkanShaderCompiler {
    public:
        VulkanShaderCompiler(VulkanRoot &);
        ~VulkanShaderCompiler();
  vk::UniqueShaderModule makeModuleFromBinary(vk::ShaderStageFlagBits,const std::string & filepath);
    private:
        VulkanRoot& m_root;
    };

}

#endif //WILLOW_VULKANSHADERCOMPILER_HPP

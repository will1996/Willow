#include "willow/rendering/Renderer.hpp"
#include "willow/root/wilo_dev_core.hpp"
#include "willow/Vulkan/VulkanRoot.hpp"
#include "willow/Vulkan/VulkanSwapchain.hpp"
#include <glfw/glfw3.h>
namespace wlo::rendering{
    class VulkanImplementation{
    public:
#ifndef NDEBUG
        VulkanImplementation(std::initializer_list<Renderer::Features>,SharedPointer<Window>,bool enableDebugging = true);
#else
        VulkanImplementation(std::initializer_list<Renderer::Features>,SharedPointer<Window>, bool enableDebugging = false);
#endif
        void setup(wlo::rendering::RenderPath);

        ~VulkanImplementation();
    private:
        wk::VulkanRoot m_vulkanRoot;
        wk::VulkanSwapchain m_swapchain;
        vk::UniqueCommandPool m_commandPool;
    };




    VulkanImplementation::~VulkanImplementation() {
    }



    VulkanImplementation::VulkanImplementation(std::initializer_list<Renderer::Features> features,SharedPointer<Window> window, bool enableDebugging):
    m_vulkanRoot(features,window.get(),enableDebugging),
    m_swapchain(m_vulkanRoot,window)
    //m_memoryManager(m_vulkanRoot)
    //m_graphicsPipelineFactory(m_vulkanRoot)
    {
        auto [graphicsIndex, presentIndex] = m_vulkanRoot.QueueFamilyIndices();
        vk::CommandPoolCreateInfo poolCreateInfo( vk::CommandPoolCreateFlags(), graphicsIndex );
        m_commandPool = m_vulkanRoot.Device().createCommandPoolUnique(poolCreateInfo);

    }

    void VulkanImplementation::setup(wlo::rendering::RenderPath) {

    }

//RENDERER


   Renderer::Renderer(SharedPointer<Window> window, std::initializer_list<RenderPath> paths,std::initializer_list<Features> Features){
       wlo::logr::initalize();
       pImpl=wlo::CreateUniquePointer<VulkanImplementation>(Features,window);
       for(auto & path : paths)
           Setup(path);
   }


    Renderer::~Renderer() {
        WILO_CORE_INFO("Renderer deconstructed");
    }


   void Renderer::Setup(const RenderPath & path) {


    }

    void Renderer::Prepare(const Frame &) {

    }

    void Renderer::Submit(const Frame &) {

    }

    auto Renderer::shareRoots() {
       return nullptr;
    }


}


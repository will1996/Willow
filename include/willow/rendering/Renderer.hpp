#pragma once
#include "willow/messaging/MessageSystem.hpp"
#include <array>
#include"RenderDataTypes.hpp"
#include"RenderPath.hpp"
#include"Frame.hpp"
#include"DataLayout.hpp"
#include <glm/glm.hpp>
namespace wlo ::rendering{
    class VulkanImplementation;

    class Renderer : wlo::MessageSystem::Observer{
    public:
        enum class Features{
            DebuggingTools,
        };
        Renderer(SharedPointer<Window>,
                std::initializer_list<RenderPath> paths = {},
                std::initializer_list<Features> features = {});
        ~Renderer();

        void Setup(const RenderPath &);//create shader modules,  graphics pipelines, renderpasses
        void Prepare(const Frame &);
        void Submit(const Frame &);//upload data to the GPU, build submit command buffers for associated Draws, present


        auto shareRoots();//give implementation class to another GPU using class in the engine


    private:
        bool m_attachmentsFinalized;
        wlo::UniquePointer<VulkanImplementation> pImpl;

    };





}
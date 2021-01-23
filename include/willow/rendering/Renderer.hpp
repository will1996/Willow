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
                std::initializer_list<Features> features = {});
        ~Renderer();

        void PrepareFrameClass(const Frame &);//needed once before submitting any frame of this class. Frames that use the same input types, number of draw calls, renderpaths needn't be set up again. 
        void Submit(const Frame &);//upload data to the GPU, build submit command buffers for associated Draws, present
        void setClearColor(wlo::Color);

        auto shareRoots();//give implementation class to another GPU using class in the engine


    private:
        bool m_attachmentsFinalized;
        wlo::UniquePointer<VulkanImplementation> pImpl;

    };





}
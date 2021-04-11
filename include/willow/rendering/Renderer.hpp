#pragma once
#include "willow/messaging/MessageSystem.hpp"
#include <array>
#include"RenderDataTypes.hpp"
#include"Frame.hpp"
#include"include/willow/data/Type.hpp"
#include "willow/rendering/PerspectiveCamera3D.hpp"
#include <glm/glm.hpp>
#include"willow/rendering/Scene.hpp"
namespace wlo ::rendering{
    class VulkanImplementation;
    struct GPUInfo{
       size_t totalMemoryUsage; 
       size_t totalMemoryAvailable;
    };



    class Renderer : wlo::MessageSystem::Observer{
    public:
        struct Statistics{
            size_t frameCount; 
            float frameRate;
        };

        enum class Features{
            DebuggingTools,
        };

        Renderer(Window&,
                std::initializer_list<Features> features = {});
        ~Renderer();
        void checkIn();
        void setMainCamera(const PerspectiveCamera3D & );

        void preAllocateScene(SceneDescription description);
        void render(const Scene &);
        void prepare(const Frame &);//needed once before submitting any frame of this class. Frames that use the same input types, number of draw calls, renderpaths needn't be set up again.
        void submit(const Frame &);//upload data to the GPU, build submit command buffers for associated Draws, present

        void setClearColor(wlo::Color);
        wlo::MessageSystem::Subject & asSubject();
        const Statistics & getStats();

    private:
        void drawScene(const Scene & );
        wlo::MessageSystem::Subject m_subject;
        wlo::UniquePointer<VulkanImplementation> pImpl;

    };





}

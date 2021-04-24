#pragma once
#include "willow/messaging/MessageSystem.hpp"
#include <array>
#include"RenderDataTypes.hpp"
#include"willow/data/Type.hpp"
#include "willow/rendering/PerspectiveCamera3D.hpp"
#include <glm/glm.hpp>
#include"willow/rendering/Scene.hpp"
#include"willow/console/Font.hpp"
#include"willow/root/EngineComponent.hpp"
namespace wlo::rendering {
    class VulkanImplementation;
    struct GPUInfo {
        size_t totalMemoryUsage;
        size_t totalMemoryAvailable;
    };



    class Renderer : public EngineComponentInstance<Renderer>{

    public:
        struct Statistics{
            size_t frameCount; 
            float frameRate;
        };

        enum class Features{
            DebuggingTools,
        };

        Renderer(ScriptEnvironment &, Window&,
                std::initializer_list<Features> features = {});
        ~Renderer();
        void checkIn();
        void setMainCamera(const PerspectiveCamera3D & );
        void preAllocateScene(SceneDescription description);

        void render(const Scene &);

        void setClearColor(wlo::Color);
        const Statistics & getStats();

    private:
        void rBeginRender();
        void nRenderComplete();

        void drawScene(const Scene & );
        wlo::UniquePointer<VulkanImplementation> pImpl;

    };





}

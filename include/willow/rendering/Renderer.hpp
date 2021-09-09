#pragma once
#include "willow/messaging/MessageSystem.hpp"
#include <array>
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



    class Renderer : public EngineComponent{

    public:
        struct Statistics{
            size_t frameCount; 
            float frameRate;
        };

        enum class Features{
            DebuggingTools,
        };

        Renderer(std::initializer_list<Features> features = {});
        ~Renderer();
        void connect(Messenger *) override;
        void registerRenderSurface(const Window & wnd);
        void draw(const RenderStart &);
        void onScreenResize(const WindowResized & msg);
        void preAllocateScene(SceneDescription description);

        void render(const Scene &,const wlo::Mat4 & cameraMatrix);

        void setClearColor(wlo::Vec4);
        const Statistics & getStats();

    private:
        void rBeginRender();
        void nRenderComplete();

        void drawScene(const Scene & );
        wlo::UniquePointer<VulkanImplementation> pImpl;

    };





}

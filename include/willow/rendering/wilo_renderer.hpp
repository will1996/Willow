/*fully abstracted top_level renderer */
#pragma once
#include "willow/rendering/wilo_rendering_API.hpp"
#include "willow/messaging/wilo_observer.hpp"
#include <array>
#include <glm/glm.hpp>
namespace wlo{
    class Renderer : public Observer{
        public:
            struct Info {
                uint32_t frameWidth;
                uint32_t frameHeight;
                bool enableGraphicsDebugging;
                bool enableRendererStatistics;
            };
        Renderer(wlo::SharedPointer<wlo::Window> p_window, wlo::Renderer::Info info);
        void initialize();
        void beginDrawCall();
        void setClearColor(glm::vec4 color);
        void setCamera(glm::mat4x4 View, glm::mat4x4 Proj);
        void pushGeometry(const std::vector<Vertex3D> & vertices, const std::vector<uint32_t> & indices, glm::mat4x4 transform);
        void submitDrawCall();

        void handleWindowResize(const WindowMessage &m);
        void reclaim();
        ~Renderer();
        private:

        struct DrawDescription{

            Vertex3D* vertices;
            size_t vertexCount;

            uint32_t* indices;
            size_t indexCount;

            glm::mat4x4 modelTransform;
        };
        

        std::vector<Vertex3D> m_vertexBuffer;

        std::vector<uint32_t> m_indexBuffer;
        
        std::array<glm::mat4x4, 3> m_uniforms;


        std::vector<DrawDescription> m_drawBuffer;

        wlo::Observer m_messageReciever;

        wlo::UniquePointer<wlo::RenderCore> p_renderCore;
        

    };

}

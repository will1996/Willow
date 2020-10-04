#include "willow/rendering/wilo_renderer.hpp"
#include "willow/root/wilo_dev_core.hpp"
namespace wlo{
    Renderer::Renderer(wlo::SharedPointer<wlo::Window> window, wlo::Renderer::Info rendererInfo)
    {
        wlo::RenderCore::Info coreInfo;
        coreInfo.enableDebugging = rendererInfo.enableGraphicsDebugging;
        coreInfo.enableProfiling = rendererInfo.enableRendererStatistics;
        coreInfo.maxVertices = 100000;

        p_renderCore = wlo::renderAPI::getRenderingCore(window, coreInfo);
        p_renderCore->initialize();
        std::string scriptPath(WILO_ENGINE_SCRIPTS_PATH);
        std::string shaderFolder = scriptPath+"../shaders/";
        p_renderCore->buildPipeline(shaderFolder+"vert.spv", shaderFolder+"frag.spv");

//        
        std::vector<Vertex3D> verticies = {
            {{-0.5f, -0.5f,0.0f},{1.0f, 0.0f, 0.0f}},
            {{0.5f, -0.5f,0.0f},{1.0f, 0.0f, 0.0f}},
            {{0.5f, 0.5f,0.0f},{1.0f, 0.0f, 0.0f}},
//            {{-0.5f, 0.5f,0.0f},{0.0f, 1.0f, 0.0f}}
        };
//        p_vertexBuffer->allocate(verticies.size());
//        p_vertexBuffer->fill(verticies,verticies.size());
//
//
//
//        p_indexBuffer = ::renderAPI::getindexBuffer(p_context);
//        
        std::vector<uint32_t> indices = {
            0,1,2//,2,3,0
        };
        pushGeometry(verticies, indices, glm::mat4(1));
//        p_indexBuffer->allocate(indices.size());
//
//        p_indexBuffer->fill(indices, indices.size());
//        
//        p_taskManager->BindVertexBuffer(p_vertexBuffer);
//        p_taskManager->BindIndexBuffer(p_indexBuffer);
//
//        WILO_CORE_INFO("Renderer initialized!");
    }

    void Renderer::initialize()
    {
    }

    void Renderer::beginDrawCall()
    {
        p_renderCore->beginDrawCall();
    }

    void Renderer::setCamera(glm::mat4x4 View, glm::mat4x4 Projection)
    {
        m_uniforms[1] = View;
        m_uniforms[2] = Projection;
    }

    void Renderer::pushGeometry(const std::vector<Vertex3D>& vertices, const std::vector<uint32_t>& indices, glm::mat4x4 transform)
    {
        DrawDescription call;
        call.vertexCount = vertices.size();
        call.vertices = 0;//&m_vertexBuffer[m_vertexBuffer.size() - 1];
        m_vertexBuffer.insert(m_vertexBuffer.end(), vertices.begin(), vertices.end());

        call.indexCount = indices.size();
        call.indices = 0;// &m_indexBuffer[m_indexBuffer.size() - 1];
        m_indexBuffer.insert(m_indexBuffer.end(), indices.begin(), indices.end());
        
        //call.modelTransform = transform;
        m_uniforms[0] = transform;
        
    }

    void Renderer::submitDrawCall()
    {
        p_renderCore->submitVertexBuffer(m_vertexBuffer, 3);
        p_renderCore->submitIndexBuffer(m_indexBuffer, 3);
        p_renderCore->submitUniforms(m_uniforms[0], m_uniforms[1], m_uniforms[2]);
        p_renderCore->submitDrawCall();
    }


    void Renderer::handleWindowResize(const wlo::Message& m){
         const wlo::WindowMessage& msg = static_cast<const WindowMessage&>(m);
         p_renderCore->resizeRenderSurface(msg.getInfo().height, msg.getInfo().width);
    }

    void Renderer::reclaim()
    {
    }

    Renderer::~Renderer(){
            //let all in-flight tasks finish up before calling quit;
        //p_taskManager->waitOnCurrentTasks();

        //p_taskManager->reclaim();
        //p_vertexBuffer->reclaim();
        //p_indexBuffer->reclaim();
        //p_frameBuffers->reclaim();
        //p_renderPass->reclaim();
        //p_graphicsPipeline->reclaim();
        //p_swapChain->reclaim();
        //p_context->reclaim();
        //WILO_CORE_INFO("Renderer reclaimed");
    }
}

#include "willow/rendering/wilo_renderer.hpp"
#include "willow/root/wilo_dev_core.hpp"
#include "glm/gtx/string_cast.hpp"
namespace wlo{
    Renderer::Renderer(wlo::SharedPointer<wlo::Window> window, wlo::Renderer::Info rendererInfo)
    {
        wlo::RenderCore::Info coreInfo;
        coreInfo.enableDebugging = rendererInfo.enableGraphicsDebugging;
        coreInfo.enableProfiling = rendererInfo.enableRendererStatistics;
        m_vertexBuffer.resize(rendererInfo.vertexBufferStartingSize);
        coreInfo.maxVertices = 500000;
        m_vertexBuffer.resize(rendererInfo.vertexBufferStartingSize);
        m_indexBuffer.resize(rendererInfo.indexBufferStartingSize);
        m_vertexBufferWritePoint = 0;
        m_indexBufferWritePoint = 0;
        p_renderCore = wlo::renderAPI::getRenderingCore(window, coreInfo);
        p_renderCore->initialize();
        std::string scriptPath(WILO_ENGINE_SCRIPTS_PATH);
        std::string shaderFolder = scriptPath+"../shaders/";
        p_renderCore->buildPipeline(shaderFolder+"vert.spv", shaderFolder+"frag.spv");
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
    void Renderer::setView(glm::mat4x4 View) {
        m_uniforms[1] = View;
    }

    void Renderer::setProjection(glm::mat4x4 Proj) {
        m_uniforms[2] = Proj;
    }

    void Renderer::pushGeometry(const std::vector<Vertex3D>& vertices, const std::vector<uint32_t>& indices, glm::mat4x4 transform)
    {
        if(m_vertexBufferWritePoint+vertices.size()<m_vertexBuffer.size())
        std::copy(vertices.begin(),vertices.end(),m_vertexBuffer.begin()+m_vertexBufferWritePoint);
        else{
            WILO_CORE_WARNING("PUSHED MORE VERTICIES TO RENDERER ( {0} ) than are available ({1})  reallocating",vertices.size(),m_vertexBuffer.size());
            resizeVertexBuffer(m_vertexBuffer.size()+vertices.size());
        }
        if(m_indexBufferWritePoint+indices.size()<m_indexBuffer.size())
            std::copy(indices.begin(),indices.end(),m_indexBuffer.begin()+m_indexBufferWritePoint);
        else{
            WILO_CORE_WARNING("PUSHED MORE INDICES TO RENDERER ( {0} ) than are available ({1}) reallocating",indices.size(),m_indexBuffer.size());
            resizeIndexBuffer(m_indexBuffer.size() + indices.size());
        }

        m_uniforms[0] = transform;
        
    }

    void Renderer::submitDrawCall()
    {
        p_renderCore->submitVertexBuffer(m_vertexBuffer, m_vertexBuffer.size());
        p_renderCore->submitIndexBuffer(m_indexBuffer, m_indexBuffer.size());
        p_renderCore->submitUniforms(m_uniforms[0], m_uniforms[1], m_uniforms[2]);
        p_renderCore->submitDrawCall();
        m_indexBufferWritePoint = 0;
        m_vertexBufferWritePoint = 0;
    }


    void Renderer::handleWindowResize(const WindowResized &m){
         const wlo::WindowMessage& msg = static_cast<const WindowMessage&>(m);
         p_renderCore->resizeRenderSurface(msg.content.width, msg.content.height);
    }

    void Renderer::reclaim()
    {
        p_renderCore->waitForLastFrame();
        p_renderCore->reclaim();
    }

    Renderer::~Renderer(){
            //let all in-flight tasks finish up before calling quit;
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

    void Renderer::setClearColor(glm::vec4 color) {
        p_renderCore->setClearColor(color);

    }

    void Renderer::clearGeometryBuffers() {
        m_vertexBuffer.assign(m_vertexBuffer.size(),Vertex3D{});
        m_indexBuffer.assign(m_indexBuffer.size(),0);
        m_indexBufferWritePoint =0;
        m_vertexBufferWritePoint = 0;
    }

    void Renderer::resizeVertexBuffer(size_t size) {
        m_vertexBuffer.resize(size);
    }

    size_t Renderer::getVertexBufferSize() {
        return m_vertexBufferWritePoint;
    }

    size_t Renderer::getVertexBufferReservedSize() {
        return m_vertexBuffer.size();
    }

    size_t Renderer::getIndexBufferSize() {
        return m_indexBufferWritePoint;
    }

    size_t Renderer::getIndexBufferReservedSize() {
        return m_indexBuffer.size();
    }

    void Renderer::resizeIndexBuffer(size_t size) {
        m_indexBuffer.resize(size);
    }


}

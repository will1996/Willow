//
// Created by W on 10/20/20.
//

#ifndef WILLOW_BLOOM_HPP
#define WILLOW_BLOOM_HPP
#include"wilo_vertex_types.hpp"
#include"willow/window/wilo_window.hpp"
#include"willow/rendering/wilo_render_core.hpp"
#include"willow/messaging/MessageSystem.hpp"
#include"Renderer.hpp"
#include"Shader.hpp"
#include"willow/root/wilo_dev_core.hpp"
#include"Objects.hpp"
namespace wlo::Bloom{
   class System : MessageSystem::Observer{
       //responsibilities:
       //provide a way to register visual representations of game objects
       //provide a way to create pipelines
       //provide a way to create shaders
       //TODO:
       //Handle framebuffering
       //allow depth buffer toggling
       //create a stencil buffer
   public:




    System();//create the rendering system, basically instantiate the back-end, detect correct API usage, etc.
    SharedPointer<Shader> createShader(std::string name,std::string filePath);
    SharedPointer<Pipeline> createPipeline(const Shader & vertexShader, const Shader & fragmentShader);
    SharedPointer<Model> createModel(std::string name, UniquePointer<std::vector<Vertex3D> >,UniquePointer<std::vector<uint32_t> > );
    UniquePointer<Renderer> createRenderer(wlo::SharedPointer<Window>,Renderer::Settings);

    ~System();
   private:
    wlo::UniquePointer<wlo::RenderCore> getRenderingCore(wlo::SharedPointer<Window> window, wlo::RenderCore::Info info);
    std::unordered_map<std::string,Shader> m_ShaderLibrary;
    std::unordered_map<std::string,Model> m_ModelLibrary;
    std::unordered_map<std::string,Pipeline> m_PipelineLibrary;
   };
}
#endif //WILLOW_BLOOM_HPP

#pragma once
#include "wilo_render_data_layout.hpp"
namespace wlo{
    class Shader{
       public:
           enum class Type {
               Vertex, Fragment, Compute
           };

        wlo::RenderDataLayout getInputDataLayout();
        void recompile();
        virtual const Type getType() const = 0;
       [[nodiscard]]  const std::vector<char>& getCode() const;
    protected:
        void compile();
        Shader(std::string shaderFilePath);
        friend class ShaderFactory;
        void readShaderFile(std::string file);
        std::vector<char> m_code;
        Type m_type;

    };

   class VertexShader : public Shader {
   public:
       VertexShader(std::string filepath);
       virtual const Shader::Type getType() const override;
   };
   class FragmentShader : public Shader {
   public:
       FragmentShader(std::string filepath);
       virtual const Shader::Type getType() const override;
   };
   class ComputeShader : public Shader {
   public:
       ComputeShader(std::string filepath);
       virtual const Shader::Type getType() const override;
   };

   class ShaderFactory {
      static VertexShader  makeVertexShader(std::string filepath);
      static FragmentShader  makeFragmentShader(std::string filepath);
      static ComputeShader  makeComputeShader(std::string filepath);
   };


}
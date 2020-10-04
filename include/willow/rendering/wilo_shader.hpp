#pragma once
#include "wilo_render_data_layout.hpp"
namespace wlo{
    class Shader{
       public:
           enum class Type {
               Vertex, Fragment, Compute
           };

        Shader(std::string shaderFilePath);
        wlo::RenderDataLayout getInputDataLayout();

        virtual void compile() = 0;

    protected:
        const std::vector<char>& getCode() const;
        const Type getType() const;
    private: 
        void readShaderFile(std::string file);
        std::vector<char> m_code;
        Type m_type;

    };


}
#include "willow/rendering/Shader.hpp"
#include "willow/rendering/RenderDataTypes.hpp"
#include <fstream>


namespace wlo{

Shader::Shader(std::string shaderFilePath){
    readShaderFile(shaderFilePath);
}


// Protected getters  
const std::vector<char>& Shader::getCode() const
{
    return m_code;
}


void Shader::readShaderFile(std::string fileFullPath ){
    std::string name = fileFullPath.substr(fileFullPath.find_last_of('.') - 4, fileFullPath.find_last_of('.'));
        if(name == "vert.spv"){
            m_type = Shader::Type::Vertex;
        }
        else if(name == "frag.spv"){
            m_type = Shader::Type::Fragment;
        }
        else{
            
            WILO_CORE_ERROR("shader file name {0} unrecognized!",name);
            assert(false);
        }

        std::ifstream shader_file (fileFullPath, std::ios::ate|std::ios::binary);
        if(!shader_file.is_open()){
            WILO_CORE_ERROR("failed to read file from path {0}",fileFullPath);
            assert(false);
        }
        m_code.resize(size_t(shader_file.tellg()));
        shader_file.seekg(0);
        WILO_ASSERT(m_code.size()!=0 && m_code.size()%4==0);
        shader_file.read(m_code.data(),m_code.size());
        shader_file.close();
}

    void Shader::recompile() {
        compile();
    }

    void Shader::compile() {
        WILO_CORE_INFO("Dummy compiliation, (true compilation unimplemented)") ;
    }

    VertexShader::VertexShader(std::string filepath) : Shader(std::move(filepath)) { }
    const Shader::Type VertexShader::getType() const {return Shader::Type::Vertex;}

    FragmentShader::FragmentShader(std::string filepath) : Shader(std::move(filepath)) { }
    const Shader::Type FragmentShader::getType() const {return Shader::Type::Vertex;}

    ComputeShader::ComputeShader(std::string filepath) : Shader(std::move(filepath)) { }
    const Shader::Type ComputeShader::getType() const {return Shader::Type::Vertex;}

    VertexShader ShaderFactory::makeVertexShader(std::string filepath) {
        return VertexShader(std::move(filepath));
    }

    FragmentShader ShaderFactory::makeFragmentShader(std::string filepath) {
        return FragmentShader(std::move(filepath));
    }

    ComputeShader ShaderFactory::makeComputeShader(std::string filepath) {
        return ComputeShader(std::move(filepath));
    }
}




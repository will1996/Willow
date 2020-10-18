#pragma once
#include <glm/glm.hpp>
#include <stdexcept>
#include "willow/root/wilo_dev_core.hpp"
/*


Vertex_Data_Types are a set of structs that together create a cross platform, clear way to send vertex information to the Shaders, graphics pipeline, and vertex buffers. 


Verticies are Structs, which follow a non-oo nameing convention based form of validation. They all must have a static member function which returns a const RenderDataLayout named Layout();
This function is invoked by the templated vertexBuffer class to check that the data it is trying to load is indeed a vertex. TODO make a way that sucks less.





*/

namespace wlo{

        //VertexElementTypeNames have 1-1 correspondance with GLM
    enum class RenderDataType {
        None = 0, f, fvec2, fvec3, fvec4, fmat2x2, fmat3x3, fmat4x4
};

    enum class RenderDataModifier{
        None = 0, Normalized 
    };
    
struct RenderDataLayoutElement{
    std::string name;
    RenderDataType type;
    RenderDataModifier modifier;
    uint32_t size;
    uint32_t offset;
        //initializer constructor
    RenderDataLayoutElement( RenderDataType i_type,const std::string& i_name, RenderDataModifier i_modifier = RenderDataModifier::None) :
        name(i_name), type(i_type), modifier(i_modifier)
    {
        size = sizeFromType(type);
    }
private: 
    static uint32_t sizeFromType(RenderDataType type){
         switch(type){
        case RenderDataType::f: return 4;
        case RenderDataType::fvec2: return 2*4;
        case RenderDataType::fvec3: return 3*4;
        case RenderDataType::fvec4: return 4*4;
        case RenderDataType::fmat2x2: return 4*4;
        case RenderDataType::fmat3x3: return 9*4;
        case RenderDataType::fmat4x4: return 16*4;

        case RenderDataType::None: return 0;
        }
         throw std::runtime_error("unknown vertexElementType");
    }
};

class RenderDataLayout{
public:
    RenderDataLayout(std::initializer_list<RenderDataLayoutElement> elements):
        m_elements(elements)
    {
        size_t currSizeCount = 0;
        for (size_t i = 0; i < m_elements.size();i++) {
            m_elements[i].offset = currSizeCount;
            currSizeCount += m_elements[i].size;
        }
        //after the loop executes currSizeCount holds the total size of the element
        m_totalSize = currSizeCount;
    }

    size_t getTotalSize(){
        return  m_totalSize;
    }

    const std::vector<RenderDataLayoutElement>& getElements() {
        return m_elements;
    }

private:
    std::vector<RenderDataLayoutElement> m_elements;
    uint32_t m_totalSize;

};







}
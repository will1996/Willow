#pragma once
#include<stdint.h>
#include<atomic>
namespace wlo{
    
 typedef uint32_t ID_type;

class ObjectCounter{
    /*quick and dirty, but also threadsafe ID maker*/
    //TODO make this actually threadsafe
    public: 
    static ID_type getCoreElementUniqueID(){return m_engineElementCounter++;}
    static ID_type getComponentUniqueID(){return m_renderObjectCounter++;}
    static ID_type getEntityUniqueID(){return m_entityCounter++;}
    private: 
    static ID_type m_engineElementCounter;
    static ID_type m_renderObjectCounter;
    static ID_type m_entityCounter;
};
}
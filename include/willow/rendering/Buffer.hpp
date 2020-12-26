//
// Created by W on 12/9/20.
//

#ifndef WILLOW_BUFFER_HPP
#define WILLOW_BUFFER_HPP
#include<vector>
#include "willow/messaging/MessageSystem.hpp"
#include "Attachment.hpp"
using std::vector;
namespace wlo::rendering{
    template<typename T>
    struct Buffer:Tag{
    public:
        Buffer(Attachment::Type type, std::initializer_list<T> init):m_type(type),m_layout(Layout<T>()),m_backing(init)
        {m_size = m_backing.size();}
        Buffer(Attachment::Type type,size_t size):m_type(type),m_layout(Layout<T>()),m_backing(size)
        {m_size = m_backing.size();}

        Attachment  attachment(){
            return Attachment(m_backing,m_type,m_size,id);
        }

        const size_t & size(){return m_size;};
//        void resize(size_t size ){m_backing.resize(size);};
        T* data() {return m_backing.data();}
        T& operator[](size_t i){
            if(i>=size())
                throw std::runtime_error("out of bounds on Buffer!");
            return m_backing[i];
        }
    private:
        size_t m_size;
        const DataLayout m_layout;
        const Attachment::Type m_type;
        vector<T> m_backing;
    };
}

#endif //WILLOW_BUFFER_HPP

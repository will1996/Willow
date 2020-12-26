//
// Created by W on 12/9/20.
//

#ifndef WILLOW_ATTACHMENT_HPP
#define WILLOW_ATTACHMENT_HPP
#include "DataLayout.hpp"
#include"RenderDataTypes.hpp"
#include "Tag.hpp"
namespace wlo::rendering{

    struct DataView{
        const byte * source;
        const DataLayout layout;
        const size_t count;
        template<typename T>
        explicit DataView(const std::vector<T>& resource):
                source(resource.data()),layout(Layout<T>()),count(resource.size())
        { }
        template<typename T>
        DataView(const std::vector<T>& resource,size_t offset, size_t count):
                source(&resource.data()[offset]),layout(Layout<T>()),count(count)
        { }

    inline    DataView(byte * data,DataLayout layout,size_t count):source(data),layout(layout),count(count){}


    };

    class Attachment{
    public:
        enum Type{
            VertexBuffer,IndexBuffer, UniformBuffer,Texture,DepthBuffer
        };
        struct Description{
            Type type;
            DataLayout layout;
            Description(Type,DataLayout);
            template<typename T>
            Description(Type attachmentType,const std::vector<T> & ):type(attachmentType),layout(Layout<T>()){};
        };
        template<typename T>
        Attachment(std::vector<T> data,Type attachmentType,const size_t & dynamicSize,size_t id):m_description(attachmentType,data),m_data((byte*)data.data()),m_count(dynamicSize),m_id(id){}
        DataView operator[](std::pair<size_t,size_t> slice){
            auto [offset,count] =slice;
            return DataView(m_data+offset, m_description.layout,count);
        }
        DataView operator[](int member){
            if(member==-1)
                return DataView(m_data,m_description.layout,m_count);
            else if(member>=0)
                return DataView(m_data+member,m_description.layout,1);
            else
                throw std::runtime_error("Invalid member access, valid values are between -1 and attachment.size(); where -1 is a view to the entire attachment");
        }
    private:
        friend class Renderer;
        Description m_description;
        byte * m_data;
        const size_t & m_count;
        const size_t m_id;
    };
}

inline bool operator==(const wlo::rendering::Attachment::Description & one,  const wlo::rendering::Attachment::Description & two){
    return (one.type==two.type) && (one.layout == two.layout);
}
#include<iostream>

inline std::ostream & operator<<(std::ostream & o, const wlo::rendering::Attachment::Description desc){
    o<<"Layout: "<<desc.layout<<" AttachmentType: "<<desc.type<<std::endl;
    return o;
}


#endif //WILLOW_ATTACHMENT_HPP

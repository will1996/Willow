//
// Created by Will Chambers on 3/4/21.
//

#ifndef WILLOW_MESH_HPP
#define WILLOW_MESH_HPP
#include"willow/rendering/RenderDataTypes.hpp"
#include "willow/root/root.hpp"
#include"willow/data/View.hpp"
namespace wlo {


    class Mesh: public Tag{
        
    public:
        template<typename T>
        Mesh(std::vector<T> vertices, std::vector<Index> indices):m_self(wlo::CreateUniquePointer<MeshModel<T>>(vertices,indices)),m_vertexType(wlo::data::Type::of<T>()),m_vertexCount(m_self->vertexView().count) {
             
        
        }

        inline  View vertexView() const{ return m_self->vertexView(); }
        inline  View indexView()const { return m_self->indexView(); }
        inline void* vertexData() { return m_self->vertexData(); }
        inline Index* indexData() { return m_self->indexData(); }

        inline void transform(glm::mat4x4 t) { return m_self->transform(t); }
        
    private:
        struct AbstractMesh {
            virtual View vertexView() const= 0;
            virtual View indexView() const= 0;

            virtual void* vertexData() = 0;
            virtual Index* indexData()= 0;

            virtual void transform(glm::mat4x4 t) = 0;
        };



		template<typename  T>
		struct MeshModel :AbstractMesh{
            MeshModel(std::vector<T> verts, std::vector<Index> inds):vertices(verts), indices(inds) { }
            View vertexView()const override {
                return View(vertices);
            };

            View indexView()const override{
                return View(indices);
            }


            void* vertexData()override{
                return vertices.data();
            }

            virtual Index* indexData() override {
                return indices.data();
            }
            
            void transform(glm::mat4x4 t) override{
                for (auto& vertex : vertices) {
                    vertex.position = t * wlo::Vec4(vertex.position,1);
                }
            }
            
			vector<T> vertices;
			vector<wlo::Index> indices;
		};


        std::unique_ptr<AbstractMesh> m_self;
        wlo::data::Type m_vertexType;
        size_t m_vertexCount;
    
    };

}

#endif //WILLOW_MESH_HPP

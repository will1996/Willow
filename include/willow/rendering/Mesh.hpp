//
// Created by Will Chambers on 3/4/21.
//

#ifndef WILLOW_MESH_HPP
#define WILLOW_MESH_HPP
#include"willow/rendering/RenderDataTypes.hpp"
#include "willow/root/root.hpp"
namespace wlo {
    template<class T>
    struct Mesh{
        vector<T> vertices;
        vector<wlo::Index> indices;
        static Mesh<T> fromFile(std::string filepath);
    };

    template<typename  T>
        inline void Transform(Mesh<T> & mesh, glm::mat4x4 transform){
            for(auto & vertex : mesh.vertices)
                    vertex.position=transform*glm::vec4{vertex.position,1};
        }
}

#endif //WILLOW_MESH_HPP

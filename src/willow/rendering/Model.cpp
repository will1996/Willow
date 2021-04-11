//
// Created by W on 1/18/21.
//

#include "willow/rendering/Model.hpp"
#define TINYOBJLOADER_IMPLEMENTATION
#include<tiny_obj_loader.h>
namespace wlo {
    template<>
    Mesh<TexturedVertex3D> Mesh<TexturedVertex3D>::fromFile(std::string filepath) {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;
        vector<wlo::Index > indices;
        vector<TexturedVertex3D> vertices;
        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str())) {
            throw std::runtime_error(warn + err);
        }
        for(const auto & shape : shapes){
            for (const auto & index : shape.mesh.indices){
               TexturedVertex3D vertex{};
               vertex.position = {
                attrib.vertices[3*index.vertex_index+0],
                attrib.vertices[3*index.vertex_index+1],
                attrib.vertices[3*index.vertex_index+2]
               };
               vertex.TexCoord = {
                  attrib.texcoords[2*index.texcoord_index +0],
                  1.0f-attrib.texcoords[2*index.texcoord_index+1]
               };
               vertices.push_back(vertex);
               indices.push_back(indices.size());
            }
        }
        return Mesh<TexturedVertex3D>{
            .vertices = vertices,
            .indices = indices
        };
    }

}
//
// Created by Will Chambers on 2/18/21.
//
#include "willow/rendering/Scene.hpp"
#include <unordered_set>
#include <unordered_map>

namespace wlo::rendering{

    std::vector<glm::mat4> & Scene::getTransforms(){
        m_packedTransformsBuffer.reserve(m_objects.size());
        for(auto & object : m_objects)
            m_packedTransformsBuffer.push_back(object.transform);
        return m_packedTransformsBuffer;
    }
    const std::vector<RenderObject> & Scene::getObjects(){
        m_packedObjectBuffer.reserve(m_objects.size());
        for(auto & object : m_objects)
            m_packedObjectBuffer.push_back(object);
        return m_packedObjectBuffer;
    }

    SceneDescription Scene::getDescription() {
        std::unordered_set<const Material*> materialSet;
        std::unordered_map<wlo::data::Type,size_t> vertexCounts;
        size_t indexCount = 0;
        for(auto & object : m_objects){
            materialSet.insert(&object.material);
            vertexCounts[object.mesh.vertexView().layout]+=object.mesh.vertexView().count;
            indexCount+=object.mesh.indexView().count;
        }
        vector<std::pair<wlo::data::Type,size_t> > finalVertexCounts;
        for(auto pair : vertexCounts)
            finalVertexCounts.push_back(pair);
        vector<const Material *> materials;
        for(auto & material : materialSet)
            materials.push_back(material);

        return SceneDescription{
            .vertexCounts = finalVertexCounts,
            .materials = materials,
            .totalIndexCount = indexCount,
        };
    }



}
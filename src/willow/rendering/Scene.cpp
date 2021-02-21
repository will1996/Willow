//
// Created by Will Chambers on 2/18/21.
//
#include "willow/rendering/Scene.hpp"

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
        for(auto object: m_objects)
        return SceneDescription{

        };
    }



}
//
// Created by Will Chambers on 2/18/21.
//
#include "willow/rendering/Scene.hpp"

namespace wlo::rendering{

    std::vector<glm::mat4> & Scene::getTransforms(){
        return m_transforms;
    }
    const std::vector<RenderObject> & Scene::getObjects(){
        return m_objects;
    }

    SceneDescription Scene::getDescription() {
        for(auto object: m_objects)
        return SceneDescription{

        };
    }

    Scene::Scene(size_t ObjectCapacity):objectCount(0),objectCapacity(ObjectCapacity) {

    }


}
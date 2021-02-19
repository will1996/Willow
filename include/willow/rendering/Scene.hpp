//
// Created by Will Chambers on 2/16/21.
//

#ifndef WILLOW_SCENE_HPP
#define WILLOW_SCENE_HPP
#include"willow/rendering/DataLayout.hpp"
#include "willow/root/Root.hpp"
#include "willow/rendering/Model.hpp"
namespace wlo::rendering {
    struct SceneDescription {
        std::vector<std::pair<DataLayout,size_t > >vertexCounts;
        std::vector<Material> materials;
        size_t totalIndexCount;
    };

    struct RenderObject:Tag{
        ModelView model;
        glm::mat4 transform = glm::mat4x4{1};
    };

class Scene{
   public:
      Scene(size_t objectCapacity = 500);
      SceneDescription getDescription();

      template<typename T>
      RenderObject* add(Model<T> & model,glm::mat4x4 transform = glm::mat4x4{1}){
          if(objectCount>objectCapacity)
              throw std::runtime_error("attempted to add more than capacity objects to the scene");
          objectCount++;
          m_objects.emplace_back(RenderObject{.model = ModelView(model),.transform = transform});
        return &m_objects.back();
      }
    std::vector<glm::mat4> & getTransforms();
    const std::vector<RenderObject> & getObjects();
   private:
    friend class Renderer;
   size_t objectCount;
   const size_t objectCapacity;
   std::vector<RenderObject> m_objects;
   std::vector<glm::mat4> m_transforms;
   };

}

#endif //WILLOW_SCENE_HPP

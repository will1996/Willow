//
// Created by Will Chambers on 2/16/21.
//

#ifndef WILLOW_SCENE_HPP
#define WILLOW_SCENE_HPP
#include"willow/data/Type.hpp"
#include "willow/root/Root.hpp"
#include"willow/rendering/Material.hpp"
#include"willow/rendering/Mesh.hpp"
#include<list>
namespace wlo::rendering {
    struct SceneDescription {
        std::vector<std::pair<wlo::data::Type,size_t > >vertexCounts;
        std::vector<const Material*> materials;
        size_t totalIndexCount;
    };

    struct RenderObject{
        const Mesh& mesh ;
        const Material& material;
        glm::mat4 transform = glm::mat4x4{1};
    };

class Scene{
   public:
      Scene() = default;
      Scene(std::vector<RenderObject> renderObjects);
      SceneDescription getDescription();

      Scene& add(const Mesh & mesh,const Material & material,glm::mat4x4 transform = glm::mat4x4{1}){
          m_objects.emplace_back(RenderObject{.mesh = mesh,.material = material,.transform = transform});
        return *this;
      }

    std::vector<glm::mat4> & getTransforms();
    const std::vector<RenderObject> & getObjects();
   private:
    friend class Renderer;
   std::list<RenderObject> m_objects;
   std::vector<RenderObject> m_packedObjectBuffer;
   std::vector<glm::mat4> m_packedTransformsBuffer;
   };

}

#endif //WILLOW_SCENE_HPP

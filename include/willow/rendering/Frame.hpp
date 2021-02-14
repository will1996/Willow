//
// Created by W on 10/30/20.
//

#ifndef WILLOW_FRAME_HPP
#define WILLOW_FRAME_HPP
#include "willow/root/Root.hpp"
#include "willow/rendering/Model.hpp"
#include "willow/rendering/DataView.hpp"
#include<variant>
namespace wlo::rendering{

    struct Viewport{
        float x;
        float y;
    };

    struct Draw {
               const DataView vertices;
               const DataView indices;
               const glm::mat4x4 modelMatrix;
               const Material material;

               template<typename T>
               Draw(const Model<T> & model,glm::mat4x4 transform):
                   vertices(model.vertices),
                   indices(model.indices),
                   modelMatrix(transform),
                   material(model.material)
                { }
    };
   class Frame{

   public:



    Frame(std::initializer_list<Draw > draws):m_draws(draws){
    }
    inline const std::vector<Draw> & getDraws() const {
       return  m_draws;
    }

   private:
       std::vector< Draw > m_draws;





   };


}


#endif //WILLOW_FRAME_HPP

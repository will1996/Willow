//
// Created by W on 10/30/20.
//

#ifndef WILLOW_FRAME_HPP
#define WILLOW_FRAME_HPP
#include "willow/root/wilo_dev_core.hpp"
#include "willow/rendering/RenderPath.hpp"
#include<variant>
namespace wlo::rendering{

    struct Viewport{
        float x;
        float y;
    };

   class Frame{

   public:
            struct TransformedGeometry{
               DataView vertices;
               DataView indices;
               const glm::mat4x4& ModelMatrix;
               vector<std::string> texturesPaths;
            };

            struct Draw {
                TransformedGeometry geo;
                const RenderPath & path;
            };


    Frame(std::initializer_list<Draw > draws):m_draws(draws){
    }
    inline const std::vector<Draw> & getDraws() const {
       return  m_draws;
    }

   private:
       std::vector< Draw > m_draws;





   };


    using Draw = Frame::Draw;


}


#endif //WILLOW_FRAME_HPP

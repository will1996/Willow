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
       struct SetClearColor {
           glm::vec4 color;
           SetClearColor(glm::vec4 Color):color(Color){};
            };
            struct Draw {
                Attachment verts;
                const RenderPath & path;
                std::vector<Attachment> attachments;
                Draw(Attachment VertexAttachment, const RenderPath & Path,std::vector<Attachment> Attachments = {} ):verts(std::move(VertexAttachment)),path(Path),attachments(std::move(Attachments)){};
            };
    using Stage = std::variant<SetClearColor,Draw>;
    Frame(std::initializer_list<Stage > stages):m_stages(stages){
    }

   private:
       std::vector< Stage > m_stages;





   };


    using Draw = Frame::Draw;
    using SetClearColor = Frame::SetClearColor;


}


#endif //WILLOW_FRAME_HPP

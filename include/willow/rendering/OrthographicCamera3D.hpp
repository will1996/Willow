//
// Created by W on 10/8/20.
//

#ifndef WILLOW_ORTHOGRAPHICCAMERA3D_HPP
#define WILLOW_ORTHOGRAPHICCAMERA3D_HPP
#include"glm/glm.hpp"
#include"willow/window/window.hpp"
#include "RenderDataTypes.hpp"
#include"willow/messaging/MessageSystem.hpp"
#undef near
#undef far
namespace wlo{
class OrthographicCamera3D: public MessageSystem::Observer {
public:
    struct BoundingBox{
       double top;
       double bottom;
       double left;
       double right;
       double near = -1;
       double far = 1;
    };
   explicit OrthographicCamera3D(BoundingBox);
  explicit OrthographicCamera3D(SharedPointer<Window> wnd,bool resizeWithWindow = true);
   void setPosition(glm::vec3 position);
   glm::mat4x4 getView();
   glm::mat4x4 getProj();
private:
    void snapTo(float width, float height);
    void resizeOn(const wlo::WindowResized &msg);
    glm::vec3 m_position;
    double m_top;
    double m_bottom;
    double m_left;
    double m_right;
    double m_near = -1;
    double m_far = 1;
};

}
template<>
inline const wlo::rendering::DataLayout Layout<wlo::OrthographicCamera3D>(){
        return wlo::rendering::DataLayout({Layout<glm::mat4x4>(),Layout<glm::mat4x4>()});
};
#endif //WILLOW_ORTHOGRAPHICCAMERA3D_HPP

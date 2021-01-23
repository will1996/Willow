//
// Created by W on 10/8/20.
//

#ifndef WILLOW_PRESPECTIVECAMERA3D_HPP
#define WILLOW_PRESPECTIVECAMERA3D_HPP
#include"glm/glm.hpp"
#include"willow/window/window.hpp"
#include "RenderDataTypes.hpp"
#include"willow/messaging/MessageSystem.hpp"
#undef near
#undef far
namespace wlo{
class PrespectiveCamera3D: public MessageSystem::Observer {
public:
   PrespectiveCamera3D();
   explicit PrespectiveCamera3D(SharedPointer<Window> wnd,bool resizeWithWindow = true);
   void moveAlongViewAxis(float distance);
   void rotate(float distance);
   void strafe(float distance);
   void setFOV(float fov);
   glm::mat4x4 getTransform() const;
private:
    void snapTo(float width, float height);
    void resizeOn(const wlo::WindowResized &msg);
    float m_aspectRatio;
    float m_FOV;
    glm::vec3 m_position;
    glm::vec3 m_focus;
    glm::vec3 m_upDirection;
};

}
template<>
inline const wlo::rendering::DataLayout Layout<wlo::PrespectiveCamera3D>(){
        return wlo::rendering::DataLayout({Layout<glm::mat4x4>(),Layout<glm::mat4x4>()});
};
#endif //WILLOW_PRESPECTIVECAMERA3D_HPP

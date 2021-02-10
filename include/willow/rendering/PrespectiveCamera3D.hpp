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
   void look(float x,float y,float sensativity = .1);
   void roll(float roll);
   void zoom(float zoom);
   void strafe(float distance);
   void setFOV(float fov);
   glm::mat4x4 getTransform() const;
   glm::vec3 getPosition() const;
private:
    void snapTo(float width, float height);
    void resizeOn(const wlo::WindowResized &msg);
    float m_aspectRatio;
    float m_FOV;
    float m_pitch;
    float m_yaw;
    float m_roll;
    glm::vec3 m_position;
    glm::vec3 m_upDirection;
    glm::vec3 m_front;
    bool firstLook;
    float lastLookX;
    float lastLookY;
    friend class CameraController;
};

}
template<>
inline const wlo::rendering::DataLayout Layout<wlo::PrespectiveCamera3D>(){
        return wlo::rendering::DataLayout({Layout<glm::mat4x4>(),Layout<glm::mat4x4>()});
};
#endif //WILLOW_PRESPECTIVECAMERA3D_HPP

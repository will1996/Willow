//
// Created by W on 10/8/20.
//

#include "willow/rendering/PrespectiveCamera3D.hpp"

namespace wlo{
    PrespectiveCamera3D::PrespectiveCamera3D()
            :m_position{0,0,0},
             m_upDirection{0,1,0},
             m_focus{0,0,1},
             m_FOV(45)
    {

    }
    PrespectiveCamera3D::PrespectiveCamera3D(SharedPointer<Window> wnd, bool resizeWithWindow)
    :m_position{0,0,-1},
     m_upDirection{0,1,0},
     m_focus{0,0,1},
     m_FOV(45)
    {
       Window::Info info = wnd->getInfo();
       m_aspectRatio = float(info.m_width)/info.m_height;
       if(resizeWithWindow)
           wnd->permit<WindowResized,PrespectiveCamera3D,&PrespectiveCamera3D::resizeOn>(this);
    }

    void PrespectiveCamera3D::setFOV(float fov) {
        m_FOV = fov;
    }


    void PrespectiveCamera3D::snapTo(float width, float height) {
        m_aspectRatio = width/height;
    }

    void PrespectiveCamera3D::resizeOn(const wlo::WindowResized & msg) {
       WILO_CORE_INFO("WINDOW RESIZED");
       snapTo(msg.content.width,msg.content.height);
    }

    glm::mat4x4 PrespectiveCamera3D::getTransform() const {
        glm::mat4x4 view = glm::lookAt(m_position, m_focus, m_upDirection);
        glm::mat4x4 projection = glm::perspective(glm::radians(45.0f), m_aspectRatio, 0.1f, 100.0f);
        return projection*view;
    }

    void PrespectiveCamera3D::moveAlongViewAxis(float distance) {
        m_position += glm::normalize(m_position-m_focus)*distance;
    }

    void PrespectiveCamera3D::rotate(float distance) {
        m_focus=glm::rotate(glm::mat4x4(),distance,m_upDirection)*glm::vec4(m_focus,1);
    }

    void PrespectiveCamera3D::strafe(float distance){
        glm::vec3 heading = glm::normalize(m_focus-m_position);
        glm::vec3 sideways = glm::cross(heading,m_upDirection);
        m_position+= distance*sideways;
    }

}

//
// Created by W on 10/8/20.
//

#include "willow/rendering/PerspectiveCamera3D.hpp"

namespace wlo{
    PerspectiveCamera3D::PerspectiveCamera3D()
            :
             m_FOV(45),
             m_pitch(0),
             m_yaw(0),
             m_roll(0),
             m_position{0,0,-1},
             m_upDirection{0,1,0},
             m_front{0,0,-1},
             m_aspectRatio(1.0),
             firstLook(true),
             lastLookX(0.0f),
             lastLookY(0.0f)
    {

    }
    PerspectiveCamera3D::PerspectiveCamera3D(Window& wnd, bool resizeWithWindow)
    :       
            m_FOV(45),
            m_pitch(0),
            m_yaw(0),
            m_roll(0),
            m_position{0,0,3}, 
            m_upDirection{0,1,0},
            m_front{0,0,-1},
            m_aspectRatio{1.0f},
            firstLook(true),
            lastLookX(0.0f),
            lastLookY(0.0f)
    {
       Window::Info info = wnd.getInfo();
       m_aspectRatio = float(info.m_width)/info.m_height;
       if(resizeWithWindow)
           wnd.permit<WindowResized,PerspectiveCamera3D,&PerspectiveCamera3D::resizeOn>(this);
    }

    void PerspectiveCamera3D::setFOV(float fov) {
        m_FOV = fov;
    }


    void PerspectiveCamera3D::snapTo(float width, float height) {
        m_aspectRatio = width/height;
    }

    void PerspectiveCamera3D::resizeOn(const wlo::WindowResized & msg) {
       snapTo(msg.content.width,msg.content.height);
    }

    glm::mat4x4 PerspectiveCamera3D::getTransform() const {
        glm::vec3 heading = glm::normalize(m_front);//vector pointing to the focus
        glm::vec3 sideways = glm::normalize(glm::cross(heading,{0,1,0}));
        glm::vec3 upDirection = glm::normalize(glm::cross(sideways, heading));

        glm::mat4x4 view = glm::lookAt(m_position, m_front+m_position, upDirection)*glm::rotate(glm::mat4x4{1},m_roll,m_front);
        glm::mat4x4 projection = glm::perspective(glm::radians(m_FOV), m_aspectRatio, 0.1f, 100.0f);
        return projection*view;
    }

    void PerspectiveCamera3D::moveAlongViewAxis(float distance) {
        m_position -= glm::normalize(m_front)*distance;
    }

    void PerspectiveCamera3D::look(float x, float y, float sens) {
        float sensativity(sens);
        if(firstLook){
            sensativity = 0;
            lastLookX = x;
            lastLookY = y;
            firstLook = false;
        }
        float xoffset = lastLookX-x;
        float yoffset = lastLookY-y;
        lastLookX = x;
        lastLookY = y;
        m_yaw -=xoffset*sensativity; 
        m_pitch +=yoffset*sensativity; 
        if(m_pitch>89.0f)
            m_pitch = 89.0f;
        if(m_pitch< -89.0f)
            m_pitch = -89.0f;

        glm::vec3 direction;
        direction.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
        direction.y = sin(glm::radians(m_pitch));
        direction.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
        m_front = glm::normalize(direction);
    }

   void PerspectiveCamera3D::roll(float roll){
//        m_roll +=roll;
//        glm::vec4 m_up4Vec{m_upDirection,0};
//        m_upDirection=glm::rotate(glm::mat4x4{1},m_roll,m_front)*m_up4Vec;
   }

   void PerspectiveCamera3D::zoom(float zoom){
    m_FOV +=zoom; 
    if (m_FOV < 1.0f)
        m_FOV = 1.0f;
    if (m_FOV > 90.0f)
        m_FOV = 90.0f; 
   }
    void PerspectiveCamera3D::strafe(float distance){
        glm::vec3 sideways = glm::normalize(glm::cross(m_front,m_upDirection));
        m_position+= distance*sideways;
    }

   glm::vec3 PerspectiveCamera3D::getPosition() const{
        return m_position; 
   }

    void PerspectiveCamera3D::moveFrameVertical(float distance) {
        m_position+=distance*m_upDirection;
    }

    void PerspectiveCamera3D::setPosition(glm::vec3 position) {
        m_position = position;
    }

}

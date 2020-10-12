//
// Created by W on 10/8/20.
//

#include "willow/rendering/camera/OrthographicCamera3D.hpp"

namespace wlo{
   OrthographicCamera3D::OrthographicCamera3D(BoundingBox viewBox) {
       m_top = viewBox.top;
       m_left = viewBox.left;
       m_right = viewBox.right;
       m_bottom = viewBox.bottom;
       m_near = viewBox.near;
       m_far = viewBox.far;
   }

    void OrthographicCamera3D::setPosition(glm::vec3 position) {
        m_position = position;
    }

    glm::mat4x4 OrthographicCamera3D::getView() {
       assert(false);
    }

    glm::mat4x4 OrthographicCamera3D::getProj() {
       return glm::transpose(glm::mat4x4{
           {2.0/(m_right-m_left),0,0,-(m_right+m_left)/(m_right-m_left)},
           {0,2.0/(m_bottom-m_top),0,-(m_top+m_bottom)/(m_bottom-m_top)},
           {0,0,2.0/(m_far-m_near),-(m_far+m_near)/(m_far-m_near)},
           {0,0,0,1}});

    }


}

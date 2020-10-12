//
// Created by W on 10/8/20.
//

#ifndef WILLOW_ORTHOGRAPHICCAMERA3D_HPP
#define WILLOW_ORTHOGRAPHICCAMERA3D_HPP
#include"glm/glm.hpp"
namespace wlo{
class OrthographicCamera3D {
public:
    struct BoundingBox{
       double top;
       double bottom;
       double left;
       double right;
       double near = -1;
       double far = 1;
    };
   OrthographicCamera3D(BoundingBox);
   void setPosition(glm::vec3 position);
   glm::mat4x4 getView();
   glm::mat4x4 getProj();
private:
    glm::vec3 m_position;
    double m_top;
    double m_bottom;
    double m_left;
    double m_right;
    double m_near = -1;
    double m_far = 1;

};
}

#endif //WILLOW_ORTHOGRAPHICCAMERA3D_HPP

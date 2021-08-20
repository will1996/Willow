//
// Created by Will Chambers on 3/4/21.
//

#include "willow/DefaultAssets.hpp"


wlo::Mesh wlo::assets::DefaultCube() {

    std::vector < wlo::TexturedVertex3D> vertices = {
    
               { .position = {-1.0f, 1.0f, -1.0},.tex = {0,0}},
               { .position = {1.0f, 1.0f, -1.0},.tex = {1,0}},
               { .position = {1.0f, -1.0f, -1.0},.tex = {1,1}},
               { .position = {-1.0f, -1.0f, -1.0},.tex = {0,1}},

               { .position = {-1.0f, 1.0f, 1.0},.tex = {0,1}},
               { .position = {1.0f, 1.0f, 1.0},.tex = {1,1}},
               { .position = {1.0f, -1.0f, 1.0},.tex = {1,0}},
               { .position = {-1.0f, -1.0f, 1.0},.tex = {0,0}},
    };

    std::vector<wlo::Index> indices = {
        // front
        0, 1, 2,
        2, 3, 0,
        // right
        1, 5, 6,
        6, 2, 1,
        // back
        7, 6, 5,
        5, 4, 7,
        // left
        4, 0, 3,
        3, 7, 4,
        // bottom
        4, 5, 1,
        1, 0, 4,
        // top
        3, 2, 6,
        6, 7, 3


    };


    return wlo::Mesh(vertices, indices);
    



};
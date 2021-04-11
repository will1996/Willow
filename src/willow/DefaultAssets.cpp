//
// Created by Will Chambers on 3/4/21.
//

#include "willow/DefaultAssets.hpp"


wlo::Mesh<wlo::TexturedVertex3D> wlo::assets::DefaultCube(){
    return wlo::Mesh<wlo::TexturedVertex3D> {
       .vertices = {
               { .position = {-1.0f, 1.0f, -1.0},.TexCoord = {0,0}},
               { .position = {1.0f, 1.0f, -1.0},.TexCoord = {1,0}},
               { .position = {1.0f, -1.0f, -1.0},.TexCoord = {1,1}},
               { .position = {-1.0f, -1.0f, -1.0},.TexCoord = {0,1}},

               { .position = {-1.0f, 1.0f, 1.0},.TexCoord = {0,1}},
               { .position = {1.0f, 1.0f, 1.0},.TexCoord = {1,1}},
               { .position = {1.0f, -1.0f, 1.0},.TexCoord = {1,0}},
               { .position = {-1.0f, -1.0f, 1.0},.TexCoord = {0,0}},
       } ,
       .indices = {
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
       }
    };



};
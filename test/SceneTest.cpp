//
// Created by Will Chambers on 2/19/21.
//

#include"willow/rendering/Scene.hpp"

int main(){
    wlo::rendering::Material cowTexture{
            .vertexShader= wlo::FileSystem::Root().append("shaders").append("vert.spv"),
            .fragmentShader=  wlo::FileSystem::Root().append("shaders").append("frag.spv"),
            .texture = wlo::FileSystem::Root().append("examples").append("Textures").append("cow.bmp")
    };

    wlo::rendering::Model<wlo::TexturedVertex3D> cube{

            .vertices = {

                    { .position = {-1.0f, 1.0f, -1.0},.TexCoord = {0,0}},
                    { .position = {1.0f, 1.0f, -1.0},.TexCoord = {1,0}},
                    { .position = {1.0f, -1.0f, -1.0},.TexCoord = {1,1}},
                    { .position = {-1.0f, -1.0f, -1.0},.TexCoord = {0,1}},

                    { .position = {-1.0f, 1.0f, 1.0},.TexCoord = {0,1}},
                    { .position = {1.0f, 1.0f, 1.0},.TexCoord = {1,1}},
                    { .position = {1.0f, -1.0f, 1.0},.TexCoord = {1,0}},
                    { .position = {-1.0f, -1.0f, 1.0},.TexCoord = {0,0}},

            },

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
            },
            .material = cowTexture
    };


    wlo::rendering::Scene scene;
   auto cube0 =  scene.add(cube);
    if(cube0->transform!=glm::mat4x4{1}) {
        std::cout<<"Scene Object transforms uninitialized"<<std::endl;
        return -1;
    }
    auto cube1 =  scene.add(cube);
    if(cube0->transform!=glm::mat4x4{1}) {
        std::cout<<"lost pointer on scene object add"<<std::endl;
        return -1;
    }

   cube1->transform = glm::rotate(glm::mat4x4{1},90.0f,{0,1,0});

    auto transforms = scene.getTransforms();
    if(transforms[0]!=glm::mat4x4{1}){
        if(transforms[0]!=glm::rotate(glm::mat4x4{1},90.0f,{0,1,0})){
            std::cout<<"transforms corrupt during getTransforms"<<std::endl;
            return -1;
        }

        std::cout<<"transforms come out in wrong order"<<std::endl;
        return -1;
    }

}
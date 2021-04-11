//
// Created by Will Chambers on 2/19/21.
//

#include"willow/rendering/Scene.hpp"
#include"willow/DefaultAssets.hpp"
int main(){
    wlo::rendering::Material cowTexture{
            .vertexShader= wlo::FileSystem::Root().append("shaders").append("vert.spv"),
            .fragmentShader=  wlo::FileSystem::Root().append("shaders").append("frag.spv"),
            .texture = wlo::FileSystem::Root().append("examples").append("Textures").append("cow.bmp")
    };

    wlo::rendering::Model<wlo::TexturedVertex3D> cube{
            .mesh = wlo::assets::DefaultCube(),
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

    auto sceneDescription = scene.getDescription();
    if(sceneDescription.totalIndexCount!=72){
        std::cout<<"Index Counts are wrong, should have been 72, was: "<<sceneDescription.totalIndexCount<<std::endl;
        return -1;
    }
    if(sceneDescription.materials.size()!=1){
        std::cout<<"Material Counts are wrong, should have been 1, was: "<<sceneDescription.materials.size()<<std::endl;
        return -1;
    }
    if(sceneDescription.materials[0]!=cowTexture){
        std::cout<<"Material is wrong, should have been CowTexture, was: "<<sceneDescription.materials[0]<<std::endl;
        return -1;
    }
    if(sceneDescription.vertexCounts.size()!=1){
        std::cout<<"vertex type count is wrong, should have been 1, was: "<<sceneDescription.vertexCounts.size()<<std::endl;
        return -1;
    }
    if(sceneDescription.vertexCounts[0].second!=16){
        std::cout<<"vertex count is wrong, should have been 16, was: "<<sceneDescription.vertexCounts[0].second<<std::endl;
        return -1;
    }
}
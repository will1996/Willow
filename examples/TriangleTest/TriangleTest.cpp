//
// Created by Will Chambers on 9/2/21.
//
#include"willow.hpp"
#include "willow/rendering/Renderer.hpp"
#include "willow/window/window.hpp"
#include "willow/root/FileSystem.hpp"
using namespace wlo::rendering;
using namespace wlo;
int main(){

    Window window({.m_width = 600, .m_height = 600, .m_title = "Triangle"});
    Renderer renderer;
    Scene scene;
    renderer.connect(&window);
    Mesh mesh(std::vector<TexturedVertex3D>{
        {.position{-.5,-.5,0},.tex{0.0,0.0}},
        {.position{-.5,.5,0},.tex{0.0,1.0}},
        {.position{.5,.5,0},.tex{0.0,0.0}},
        {.position{.5,-.5,0},.tex{0.0,0.0}},
    },{0,1,2,2,3,0});
    Texture kitTexture(wlo::FileSystem::Texture("kit.jpeg"));
    Material mat = {.vertexShader = wlo::FileSystem::Shader("shader.vert"),.fragmentShader = wlo::FileSystem::Shader("shader.frag"),.texture = kitTexture};

    scene.add(mesh,mat,glm::mat4(1));
    scene.add(mesh,mat,glm::translate(glm::mat4(1),{0,.01,0}));

    renderer.preAllocateScene(scene.getDescription());
    PerspectiveCamera3D mainCamera;
    renderer.setMainCamera(mainCamera);
    while(not window.shouldClose()){

        renderer.render(scene);
    }




}

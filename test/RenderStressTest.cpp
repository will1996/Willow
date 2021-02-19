//
// Created by W on 10/11/20.
//

int main(){
    return 0;
}
//#include "willow.hpp"
//#include"glm/gtc/matrix_transform.hpp"
//#include"willow/rendering/RenderDataTypes.hpp"
//#include"willow/rendering/Renderer.hpp"
//
//
//class WillowRenderStressTester : public wlo::Application{
//public:
//    WillowRenderStressTester(std::string rootStr): Application(rootStr){}
//
//    std::vector<wlo::Vertex3D> rotateTris(std::vector<wlo::Vertex3D> tri, float angle) {
//        return {
//           glm::rotate(glm::mat4x4(1),angle,glm::vec3(0,0,1))*tri[0],
//           glm::rotate(glm::mat4x4(1),angle,glm::vec3(0,0,1))*tri[1],
//           glm::rotate(glm::mat4x4(1),angle,glm::vec3(0,0,1))*tri[2]
//        };
//    }
//
//
//    std::vector<size_t> triInds(size_t offset){
//        return {
//           0+offset,
//           1+offset,
//           2+offset
//        };
//    }
//
//    void setup() override {
//
//        struct LightPosition2D{
//            float x, y;
//        };
//
//        struct lightDirection{
//            float alpha;
//        };
//
//
//
//
//        //float angle = 0;
//
//        //while (!m_shutting_down) {
//        //    std::vector<size_t> inds = triInds(triangles.size());
//        //    indices.insert(indices.end(),inds.begin(),inds.end());
//        //    std::vector<wlo::Vertex3D> tris = rotateTris(triangle,angle);
//        //    triangles.insert(triangles.end(), tris.begin(), tris.end());
//
//        //    m_renderer->beginDrawCall();
//        //    glm::mat4x4 view = glm::mat4x4(1);
//        //    glm::mat4x4 proj = glm::mat4x4(1);//glm::ortho(0.0f, 600.0f, 0.0f, 800.0f);
//        //    glm::mat4x4 model = glm::rotate(glm::mat4x4(1), glm::radians(90.0f), {0, 0, 1});
//        //    m_renderer->setCamera(view, proj);
//        //    m_renderer->pushGeometry(triangles, indices, model);
//        //    m_renderer->submitDrawCall();
//        //    m_main_window->checkIn();
//        //    angle+=1;
//        //}
//        //std::cout<<"PUSHED: "<<triangles.size() <<"Verticies!!!"<<std::endl;
//        //std::cout<<"PUSHED: "<<indices.size() <<"Indices!!!"<<std::endl;
//    }
//};
//
//
//
//
//
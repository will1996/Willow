//
// Created by W on 12/5/20.
//
#include"willow/rendering/DataLayout.hpp"
#include"willow/rendering/RenderDataTypes.hpp"
#include"willow/rendering/Attachment.hpp"
#include<iostream>
using namespace wlo::rendering;
using namespace std;
int main(){
DataLayout layout({{DataLayout::DataType::Float,4} });
cout<<layout<<endl;

cout<<DataLayout::fromFundamental<float>()<<endl;
cout<<DataLayout::fromFundamental<size_t>()<<endl;
cout<<DataLayout::fromFundamental<double>()<<endl;

cout<<wlo::Vertex3D::Layout()<<endl;


cout<<"size of size_t: "<<sizeof(size_t)<<endl;
cout<<"size of int: "<<sizeof(int)<<endl;
cout<<"size of long: "<<sizeof(long)<<endl;

cout<<"using the short hand Layout functino"<<endl;
cout<<Layout<float>()<<endl;
cout<<Layout<size_t>()<<endl;
cout<<Layout<wlo::Vertex3D>()<<endl;

std::stringstream ss;
ss<<Layout<float>()<<endl;

Attachment::Description desc(Attachment::Type::VertexBuffer,Layout<wlo::Vertex3D>());
ss<<desc<<endl;

if(Layout<wlo::Vertex2D>() != Layout<wlo::Vertex3D>())
    cout<<"LAYOUT COMPARISON Vert2D to Vert3D CORRECTLY FAILS"<<endl;
if(Layout<wlo::Vertex3D>() == Layout<wlo::Vertex3D>())
    cout<<"LAYOUT COMPARISON Vert3D to Vert3D CORRECTLY PASSES"<<endl;


cout<<"description"<<ss.str()<<endl;






}
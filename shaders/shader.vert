#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec4 pos;
layout (location = 1) in vec4 inColor;

layout (binding =0) uniform buffer{
 mat4  mvcMatrix;
}cameraTransform;

layout (push_constant) uniform push_constant {
mat4 matrix;
} modelMatrix;

layout (location = 0) out vec4 outColor;
void main()
{
    outColor = inColor;
    gl_Position = cameraTransform.mvcMatrix*modelMatrix.matrix * pos;
}
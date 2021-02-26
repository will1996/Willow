#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;

layout (binding =0) uniform buffer{
 mat4  mvcMatrix;
}cameraTransform;

layout (push_constant) uniform push_constant {
mat4 matrix;
} modelMatrix;

layout (location = 0) out vec2 outTex;
void main()
{
    outTex = tex;
    gl_Position = cameraTransform.mvcMatrix*modelMatrix.matrix * vec4(pos,1);
}
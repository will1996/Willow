#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (location = 0) in vec2 texCoord;

layout (location = 0) out vec4 outColor;
layout(set = 0, binding = 1) uniform sampler2D tex1;
void main()
{
    vec3 color = texture(tex1,texCoord).xyz;
    outColor = vec4(color,1.0);
}
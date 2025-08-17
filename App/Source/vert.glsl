#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec2 TexCoord;

void main()
{
    
    TexCoord = vec2(aPos.x, aPos.y);
    gl_Position = vec4(aPos.x, 0. - aPos.y, aPos.z, 1.0);
}       

#version 330 core
out vec4 FragColor;  
in vec3 ourColor;

uniform float iTime;
  
void main()
{
    FragColor = vec4(ourColor * sin(iTime), 1.0);
}
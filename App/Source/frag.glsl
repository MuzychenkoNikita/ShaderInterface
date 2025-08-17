#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
vec2 FragCoord = gl_FragCoord.xy;

uniform float iTime;
uniform vec2 iResolution;

void main()
{
    vec2 uv = FragCoord/iResolution.xy;
    vec3 col = 0.5 + 0.5*cos(iTime+uv.xyx+vec3(0,2,4));
    FragColor = vec4(uv, 0., 1.0);
}

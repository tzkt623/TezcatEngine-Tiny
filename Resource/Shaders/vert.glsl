#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec4 aColor;
layout (location = 3) in vec2 aUV;

uniform mat4 TINY_MatP;
uniform mat4 TINY_MatV;
uniform mat4 TINY_MatM;

out vec4 myColor;
out vec2 myUV;

void main()
{
    gl_Position = TINY_MatP * TINY_MatV * TINY_MatM * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    myColor = aColor;
    myUV = aUV;
}
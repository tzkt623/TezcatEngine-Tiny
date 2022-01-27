#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec4 aColor;

uniform mat4 PMatrix;
uniform mat4 VMatrix;
uniform mat4 MMatrix;

out vec4 myColor;

void main()
{
//    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);

    gl_Position = PMatrix * VMatrix * MMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    myColor = aColor;
};
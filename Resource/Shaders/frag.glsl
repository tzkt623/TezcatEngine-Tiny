#version 330 core

in vec4 myColor;
in vec2 myUV;

uniform sampler2D TINY_TexColor;

out vec4 myFinalColor;

void main()
{
    //FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
    myFinalColor = myColor * texture(TINY_TexColor, myUV);
} 
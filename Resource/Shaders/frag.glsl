#version 330 core
in vec4 myColor;

out vec4 FragColor;
in vec2 myUV;

uniform sampler2D texColor;

void main()
{
    //FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
    FragColor = texture(texColor, myUV);
} 
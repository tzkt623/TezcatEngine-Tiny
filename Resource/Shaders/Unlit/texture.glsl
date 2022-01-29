TINY_BEGIN_DEF
{
    Name = Unlit/Texture;
    OrderID = 0;
}
TINY_END_DEF

TINY_BEGIN_VS
{
    layout (location = 0) in vec3 aPos;
    layout (location = 3) in vec2 aUV;

    uniform mat4 TINY_MatP;
    uniform mat4 TINY_MatV;
    uniform mat4 TINY_MatM;

    out vec2 myUV;

    void main()
    {
        vec4 position =  vec4(aPos, 1.0);
        gl_Position = TINY_MatP * TINY_MatV * TINY_MatM * position;

        myUV = aUV;
    }
}
TINY_END_VS

TINY_BEGIN_FS
{
    in vec4 myColor;
    in vec2 myUV;

    uniform sampler2D TINY_TexColor;
    out vec4 myFinalColor;

    void main()
    {
        myFinalColor = texture(TINY_TexColor, myUV);
    }
}
TINY_END_FS
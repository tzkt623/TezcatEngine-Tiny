#TINY_HEAD_BEGIN
{
    str Name = Unlit/Texture;
}
#TINY_HEAD_END

#TINY_PASS_BEGIN
{
    #TINY_CFG_BEGIN
    {
        int Version = 330;
        int OrderID = 0;
        str Queue = Opaque;
    }
    #TINY_CFG_END

    #TINY_VS_BEGIN
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
    #TINY_VS_END

    #TINY_FS_BEGIN
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
    #TINY_FS_END
}
#TINY_PASS_END
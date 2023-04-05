#TINY_HEAD_BEGIN
{
    str Name = Unlit/Texture;
}
#TINY_HEAD_END

#TINY_PASS_BEGIN
{
    #TINY_CFG_BEGIN
    {
        str Name = Texture;
        int Version = 330;
        int OrderID = 0;
        str Queue = Opaque;
        str DepthTest = Less;
        bool ZWrite = true;
        str DepthTest = Less;
        str CullFace = Back;
    }
    #TINY_CFG_END

    #TINY_VS_BEGIN
    {
        layout (location = 0) in vec3 aPos;
        layout (location = 3) in vec2 aUV;

        uniform mat4 TINY_MatrixP;
        uniform mat4 TINY_MatrixV;
        uniform mat4 TINY_MatrixM;

        out vec2 myUV;

        void main()
        {
            gl_Position = TINY_MatrixP * TINY_MatrixV * TINY_MatrixM * vec4(aPos, 1.0);
            myUV = aUV;
        }
    }
    #TINY_VS_END

    #TINY_FS_BEGIN
    {
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
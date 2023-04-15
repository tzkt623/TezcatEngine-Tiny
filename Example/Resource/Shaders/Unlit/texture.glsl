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
        int OrderID = 50;
        str Queue = Opaque;
        str DepthTest = Less;
        bool ZWrite = true;
        str DepthTest = Less;
        str CullFace = Off;
    }
    #TINY_CFG_END

    #TINY_VS_BEGIN
    {
        #include "tiny_vs_base"

        layout (location = 0) in vec3 aPos;
        layout (location = 3) in vec2 aUV;

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
        #include "tiny_fs_texture"

        in vec2 myUV;
        out vec4 myFinalColor;

        void main()
        {
            myFinalColor = texture(TINY_TexColor, myUV);
        }
    }
    #TINY_FS_END
}
#TINY_PASS_END
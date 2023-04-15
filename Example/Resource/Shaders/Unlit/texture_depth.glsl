#TINY_HEAD_BEGIN
{
    str Name = Unlit/TextureDepth;
}
#TINY_HEAD_END

#TINY_PASS_BEGIN
{
    #TINY_CFG_BEGIN
    {
        str Name = TextureDepth;
        int Version = 330;
        int OrderID = 50;
        str Queue = Opaque;
        str DepthTest = Less;
        bool ZWrite = true;
        str CullFace = Back;
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
        #include "tiny_fs_camera"
        #include "tiny_fs_function"

        out vec4 myFinalColor;
        in vec2 myUV;

        void main()
        {
            float z = texture(TINY_TexColor, myUV).r;
            myFinalColor = vec4(z, z, z, 1.0f);
        }
    }
    #TINY_FS_END
}
#TINY_PASS_END
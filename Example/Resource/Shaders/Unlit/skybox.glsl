#TINY_HEAD_BEGIN
{
    str Name = Unlit/Skybox;
}
#TINY_HEAD_END

#TINY_PASS_BEGIN
{
    #TINY_CFG_BEGIN
    {
        str Name = Skybox;
        int Version = 330;
        int OrderID = 50;
        str Queue = Background;
        str DepthTest = Less;
        bool ZWrite = false;
        str DepthTest = Less;
        str CullFace = Back;
    }
    #TINY_CFG_END

    #TINY_VS_BEGIN
    {
        #include "tiny_vs_skybox"

        layout (location = 0) in vec3 aPos;
        out vec3 myUV;

        void main()
        {
            myUV = aPos;
            gl_Position = TINY_MatrixP * TINY_MatrixSBV * vec4(aPos, 1.0);
        }
    }
    #TINY_VS_END

    #TINY_FS_BEGIN
    {
        #include "tiny_fs_texture"
        
        in vec3 myUV;
        out vec4 myFinalColor;

        void main()
        {
            myFinalColor = texture(TINY_TexCube, myUV);
        }
    }
    #TINY_FS_END
}
#TINY_PASS_END
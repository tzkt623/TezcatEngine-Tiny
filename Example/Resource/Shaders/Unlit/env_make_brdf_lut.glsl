#TINY_HEAD_BEGIN
{
    str Name = Unlit/EnvMakeBRDFLut;
}
#TINY_HEAD_END

#TINY_PASS_BEGIN
{
    #TINY_CFG_BEGIN
    {
        str Name = EnvMakeBRDFLut;
        int Version = 330;
        int OrderID = 50;
        str Queue = Prepare;
        str DepthTest = Off;
        bool ZWrite = true;
        str CullFace = Back;
    }
    #TINY_CFG_END

    #TINY_VS_BEGIN
    {
        #include "tiny_vs_base"

        layout (location = 0) in vec3 aPos;
        layout (location = 2) in vec2 aUV;

        out vec2 myUV;

        void main()
        {
            myUV = aUV;
            gl_Position = vec4(aPos, 1.0);
        }
    }
    #TINY_VS_END

    #TINY_FS_BEGIN
    {
        #include "tiny_fs_texture"
        #include "tiny_fs_pbr_func"
        
        in vec2 myUV;
        out vec2 myFinalColor;

        void main()
        {
            myFinalColor = integrateBRDF(myUV.x, myUV.y);
        } 
    }
    #TINY_FS_END
}
#TINY_PASS_END
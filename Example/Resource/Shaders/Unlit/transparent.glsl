#TINY_HEAD_BEGIN
{
    str Name = Unlit/Transparent;
}
#TINY_HEAD_END

#TINY_PASS_BEGIN
{
    #TINY_CFG_BEGIN
    {
        str Name = Transparent;
        int Version = 330;
        int OrderID = 50;
        str Queue = Transparent;
        str DepthTest = Less;
        bool ZWrite = false;
        str CullFace = Off;
        bool Blend = true;
        str BlendSrc = SrcA;
        str BlendTar = 1-SrcA;
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
            vec4 fcolor = texture(TINY_TexColor, myUV);
//            if(fcolor.a < 0.1)
//            {
//                discard;
//            }
            myFinalColor = fcolor;
            //myFinalColor = vec4(vec3(linearizeDepth(gl_FragCoord.z) / far), 1.0);
        }
    }
    #TINY_FS_END
}
#TINY_PASS_END
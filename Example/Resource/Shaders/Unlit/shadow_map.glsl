#TINY_HEAD_BEGIN
{
    str Name = Unlit/ShadowMap;
}
#TINY_HEAD_END

#TINY_PASS_BEGIN
{
    #TINY_CFG_BEGIN
    {
        str Name = ShadowMap;
        int Version = 330;
        int OrderID = 0;
        str Queue = None;
        str DepthTest = Less;
        bool ZWrite = true;
        str CullFace = Back;
    }
    #TINY_CFG_END

    #TINY_VS_BEGIN
    {
        #include "tiny_vs_base"
        #include "tiny_vs_shadow"

        layout (location = 0) in vec3 aPos;
        
        void main()
        {
            gl_Position = TINY_MatrixLit * TINY_MatrixM * vec4(aPos, 1.0);
        }
    }
    #TINY_VS_END

    #TINY_FS_BEGIN
    {
        void main()
        {
            //gl_FragDepth = gl_FragCoord.z;
        }
    }
    #TINY_FS_END
}
#TINY_PASS_END
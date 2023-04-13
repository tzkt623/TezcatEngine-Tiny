#TINY_HEAD_BEGIN
{
    str Name = Unlit/Shadow;
}
#TINY_HEAD_END

#TINY_PASS_BEGIN
{
    #TINY_CFG_BEGIN
    {
        str Name = Shadow;
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
        layout (location = 0) in vec3 aPos;

        uniform mat4 TINY_MatrixP;
        uniform mat4 TINY_MatrixV;
        uniform mat4 TINY_MatrixM;

        void main()
        {
            gl_Position = TINY_MatrixP * TINY_MatrixV * TINY_MatrixM * vec4(aPos, 1.0);
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
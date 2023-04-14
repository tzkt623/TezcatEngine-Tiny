#TINY_HEAD_BEGIN
{
    str Name = Unlit/ColorDepth;
}
#TINY_HEAD_END

#TINY_PASS_BEGIN
{
    #TINY_CFG_BEGIN
    {
        str Name = ColorDepth;
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
        out vec4 myFinalColor;
        uniform vec2 TINY_ViewNearFar;

        float linearizeDepth(float depth) 
        {
            float z = depth * 2.0 - 1.0; // back to NDC 
            return (2.0 * TINY_ViewNearFar.x * TINY_ViewNearFar.y) / (TINY_ViewNearFar.y + TINY_ViewNearFar.x - z * (TINY_ViewNearFar.y - TINY_ViewNearFar.x));    
        }

        void main()
        {
            myFinalColor = vec4(vec3(linearizeDepth(gl_FragCoord.z) / TINY_ViewNearFar.y), 1.0);
        }
    }
    #TINY_FS_END
}
#TINY_PASS_END
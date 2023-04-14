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
        out vec4 myFinalColor;
        in vec2 myUV;

        uniform sampler2D TINY_TexColor;
        uniform vec2 TINY_ViewNearFar;

        float linearizeDepth(float depth) 
        {
            float z = depth * 2.0 - 1.0; // back to NDC
            return (2.0 * TINY_ViewNearFar.x * TINY_ViewNearFar.y) / (TINY_ViewNearFar.y + TINY_ViewNearFar.x - z * (TINY_ViewNearFar.y - TINY_ViewNearFar.x));    
        }

        void main()
        {
            float z = texture(TINY_TexColor, myUV).r;
            myFinalColor = vec4(z, z, z, 1.0f);
        }
    }
    #TINY_FS_END
}
#TINY_PASS_END
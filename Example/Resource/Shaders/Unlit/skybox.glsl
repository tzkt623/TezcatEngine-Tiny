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
        str DepthTest = LessEqual;
        bool ZWrite = false;
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
        uniform bool TINY_IsHDR;
        out vec4 myFinalColor;

        void main()
        {
            vec3 color = texture(TINY_TexCube, myUV).rgb;
            if(TINY_IsHDR)
            {
                color = color / (color + vec3(1.0));
                color = pow(color, vec3(1.0/2.2)); 
            }
            myFinalColor = vec4(color, 1.0);
        }
    }
    #TINY_FS_END
}
#TINY_PASS_END
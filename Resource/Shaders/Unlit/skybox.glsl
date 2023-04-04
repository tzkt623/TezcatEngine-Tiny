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
        int OrderID = 0;
        str Queue = Background;
        bool ZWrite = false;
        str DepthTest = Less;
        str CullFace = Back;
    }
    #TINY_CFG_END

    #TINY_VS_BEGIN
    {
        layout (location = 0) in vec3 aPos;

        uniform mat4 TINY_MatrixP;
        uniform mat4 TINY_MatrixV;

        out vec3 myUV;

        void main()
        {
            mat4 v = mat4(mat3(TINY_MatrixV));
            myUV = aPos;
            gl_Position = TINY_MatrixP * v * vec4(aPos, 1.0);
        }
    }
    #TINY_VS_END

    #TINY_FS_BEGIN
    {
        in vec3 myUV;
        uniform samplerCube TINY_TexCube;
        out vec4 myFinalColor;

        void main()
        {
            myFinalColor = texture(TINY_TexCube, myUV);
        }
    }
    #TINY_FS_END
}
#TINY_PASS_END
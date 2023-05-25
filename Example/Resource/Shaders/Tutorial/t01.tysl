#TINY_HEAD_BEGIN
{
    str Name = Tutorial/t01;
}
#TINY_HEAD_END

#TINY_PASS_BEGIN
{
    #TINY_CFG_BEGIN
    {
        str Name = Color;
        int Version = 330;
        int OrderID = 50;
        str LightMode = Unlit;
        str Queue = Prepare;
        str DepthTest = Off;
        bool ZWrite = false;
        str CullFace = Off;
    }
    #TINY_CFG_END

    #TINY_VS_BEGIN
    {
        layout (location = 0) in vec3 aPos;

        void main()
        {
            gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
        }
    }
    #TINY_VS_END

    #TINY_FS_BEGIN
    {
        out vec4 myFinalColor;

        void main()
        {
            myFinalColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
        } 
    }
    #TINY_FS_END
}
#TINY_PASS_END
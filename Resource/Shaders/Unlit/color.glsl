#TINY_HEAD_BEGIN
{
    str Name = Unlit/Color;
}
#TINY_HEAD_END

#TINY_PASS_BEGIN
{
    #TINY_CFG_BEGIN
    {
        int Version = 330;
        int OrderID = 0;
        str Queue = Opaque;
        bool ZWrite = true;
        str CullFace = Back;
    }
    #TINY_CFG_END

    #TINY_VS_BEGIN
    {
        layout (location = 0) in vec3 aPos;
        layout (location = 2) in vec4 aColor;

        uniform mat4 TINY_MatP;
        uniform mat4 TINY_MatV;
        uniform mat4 TINY_MatM;

        out vec4 myColor;

        void main()
        {
            vec4 position =  vec4(aPos, 1.0);
            gl_Position = TINY_MatP * TINY_MatV * TINY_MatM * position;
            
            myColor = aColor;
        }
    }
    #TINY_VS_END

    #TINY_FS_BEGIN
    {
        in vec4 myColor;

        out vec4 myFinalColor;

        void main()
        {
            myFinalColor = myColor;
        } 
    }
    #TINY_FS_END
}
#TINY_PASS_END
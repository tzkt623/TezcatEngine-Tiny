#TINY_HEAD_BEGIN
{
    str Name = Unlit/Color;
}
#TINY_HEAD_END

#TINY_PASS_BEGIN
{
    #TINY_CFG_BEGIN
    {
        str Name = Color;
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
        #include "tiny_vs_base"

        layout (location = 0) in vec3 aPos;
        layout (location = 2) in vec4 aColor;

        out vec4 myColor;
        out vec4 myWorldPosition;

        void main()
        {
            vec4 position =  vec4(aPos, 1.0);
            gl_Position = TINY_MatrixP * TINY_MatrixV * TINY_MatrixM * position;
            
            myColor = aColor;
            myWorldPosition = TINY_MatrixM * position;
        }
    }
    #TINY_VS_END

    #TINY_FS_BEGIN
    {
        in vec4 myColor;
        in vec4 myWorldPosition;
        out vec4 myFinalColor;

        void main()
        {
            //myFinalColor = myColor;
            myFinalColor = vec4(vec3(242,141,0)/255.0, 1.0);
            //myFinalColor = vec4(myWorldPosition.xyz / myWorldPosition.w, 1.0f);
        } 
    }
    #TINY_FS_END
}
#TINY_PASS_END
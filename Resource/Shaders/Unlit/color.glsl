TINY_BEGIN_DEF
{
    Name = Unlit/Color;
    OrderID = 0;
}
TINY_END_DEF

TINY_BEGIN_VS
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
TINY_END_VS

TINY_BEGIN_FS
{
    in vec4 myColor;

    out vec4 myFinalColor;

    void main()
    {
        myFinalColor = myColor;
    } 
}
TINY_END_FS
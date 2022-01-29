TINY_BEGIN_DEF
{
    Name = Standard/Std1;
    OrderID = 0;
}
TINY_END_DEF

TINY_BEGIN_VS
{
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aNormal;
    layout (location = 2) in vec4 aColor;
    layout (location = 3) in vec2 aUV;

    uniform mat4 TINY_MatP;
    uniform mat4 TINY_MatV;
    uniform mat4 TINY_MatM;

    out vec4 myColor;
    out vec2 myUV;
    out vec3 myNormal;
    out vec3 myWorldPosition;

    void main()
    {
        vec4 position =  vec4(aPos, 1.0);
        gl_Position = TINY_MatP * TINY_MatV * TINY_MatM * position;
        
        myColor = aColor;
        myUV = aUV;
        myNormal = aNormal;
        myWorldPosition = vec3(TINY_MatM * position);
    }
}
TINY_END_VS

TINY_BEGIN_FS
{
    in vec4 myColor;
    in vec2 myUV;
    in vec3 myNormal;
    in vec3 myWorldPosition;

    uniform sampler2D TINY_TexColor;

    out vec4 myFinalColor;

    vec3 lightPosition = vec3(1,1,1);
    vec3 lightColor = vec3(1,1,1);

    void main()
    {
        vec3 normal = normalize(myNormal);
        vec3 lightDir = normalize(vec3(1,1,1) - myWorldPosition);
        float diff = max(dot(normal, lightDir), 0.0);
        vec3 diffuse = diff * lightColor;

        //FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
        myFinalColor = diff * myColor * texture(TINY_TexColor, myUV);
    }
}
TINY_END_FS
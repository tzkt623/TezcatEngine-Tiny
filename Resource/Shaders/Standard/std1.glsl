#TINY_HEAD_BEGIN
{
    str Name = Standard/Std1;
}
#TINY_HEAD_END

#TINY_PASS_BEGIN
{
    #TINY_CFG_BEGIN
    {
        int Version = 330;
        int OrderID = 0;
        str Queue = Opaque;
    }
    #TINY_CFG_END

    #TINY_VS_BEGIN
    {
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aNormal;
        layout (location = 2) in vec4 aColor;
        layout (location = 3) in vec2 aUV;

        uniform mat4 TINY_MatP;
        uniform mat4 TINY_MatV;
        uniform mat4 TINY_MatM;
        uniform mat3 TINY_MatN;

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
            myNormal = TINY_MatN * aNormal;
            myWorldPosition = vec3(TINY_MatM * position);
        }
    }
    #TINY_VS_END

    #TINY_FS_BEGIN
    {
        in vec4 myColor;
        in vec2 myUV;
        in vec3 myNormal;
        in vec3 myWorldPosition;

        uniform float TINY_AmbientStrength = 0.1f;
        uniform vec3 TINY_LightPosition = vec3(0.0f, 0.0f, 0.0f);
        uniform vec3 TINY_LightColor = vec3(1.0f, 1.0f, 1.0f);
        uniform vec3 TINY_ViewPosition;
        uniform sampler2D TINY_TexColor;

        out vec4 myFinalColor;

        float specularStrength = 0.5;

        void main()
        {
            vec3 ambient = TINY_AmbientStrength * TINY_LightColor;

            vec3 normal = normalize(myNormal);
            vec3 lightDir = normalize(TINY_LightPosition - myWorldPosition);
            float diff = max(dot(normal, lightDir), 0.0);
            vec3 diffuse = diff * TINY_LightColor;

            vec3 viewDir = normalize(TINY_ViewPosition - myWorldPosition);
            vec3 reflectDir = reflect(-lightDir, normal);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
            vec3 specular = specularStrength * spec * TINY_LightColor;

            myFinalColor = vec4(ambient + diffuse + specular, 1.0f) * myColor * texture(TINY_TexColor, myUV);
            //myFinalColor = vec4(ambient + diffuse + specular, 1.0f);
        }
    }
    #TINY_FS_END
}
#TINY_PASS_END
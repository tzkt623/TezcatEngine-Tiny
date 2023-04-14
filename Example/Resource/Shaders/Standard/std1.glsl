#TINY_HEAD_BEGIN
{
    str Name = Standard/Std1;
}
#TINY_HEAD_END

#TINY_PASS_BEGIN
{
    #TINY_CFG_BEGIN
    {
        str Name = Std1;
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
        layout (location = 1) in vec3 aNormal;
        layout (location = 2) in vec4 aColor;
        layout (location = 3) in vec2 aUV;

        uniform mat4 TINY_MatrixP;
        uniform mat4 TINY_MatrixV;
        uniform mat4 TINY_MatrixM;
        uniform mat3 TINY_MatrixN;

        out vec4 myColor;
        out vec2 myUV;
        out vec3 myNormal;
        out vec3 myWorldPosition;

        void main()
        {
            vec4 position =  vec4(aPos, 1.0);
            gl_Position = TINY_MatrixP * TINY_MatrixV * TINY_MatrixM * position;

            myColor = aColor;
            myUV = aUV;
            myNormal = TINY_MatrixN * aNormal;
            myWorldPosition = vec3(TINY_MatrixM * position);
        }
    }
    #TINY_VS_END

    #TINY_FS_BEGIN
    {
        struct StdMaterial
        {
            sampler2D diffuse;
            sampler2D normal;
            sampler2D specular;
            float shininess;
        };

        struct LightDirection
        {
            vec3 direction;
            vec3 ambient;
            vec3 diffuse;
            vec3 specular;
        };

        in vec4 myColor;
        in vec2 myUV;
        in vec3 myNormal;
        in vec3 myWorldPosition;

        uniform StdMaterial TINY_MatStd;
        uniform LightDirection TINY_LitDir;

        uniform samplerCube TINY_TexCube;
        uniform vec3 TINY_ViewPosition;

        out vec4 myFinalColor;

        vec4 reflection(vec3 I)
        {
            //vec3 I = normalize(myWorldPosition - TINY_ViewPosition);
            vec3 R = reflect(I, normalize(myNormal));
            return vec4(texture(TINY_TexCube, R).rgb, 1.0);
        }

        vec4 refraction(vec3 I)
        {
            float ratio = 1.00 / 1.52;
            //vec3 I = normalize(myWorldPosition - TINY_ViewPosition);
            vec3 R = refract(I, normalize(myNormal), ratio);
            return vec4(texture(TINY_TexCube, R).rgb, 1.0);
        }

        vec3 calcDirectionLight(LightDirection lit, vec3 viewDir, vec3 normal)
        {
            vec3 light_dir = normalize(-lit.direction);
            float diff = max(dot(normal, light_dir), 0.0);
            vec3 reflect_dir = reflect(-light_dir, normal);
            float spec = pow(max(dot(viewDir, reflect_dir), 0.0), TINY_MatStd.shininess);

            vec3 ambient = lit.ambient * texture(TINY_MatStd.diffuse, myUV).rgb;
            vec3 diffuse = lit.diffuse * diff * texture(TINY_MatStd.diffuse, myUV).rgb;
            vec3 specular = lit.specular * spec * texture(TINY_MatStd.specular, myUV).rrr;

            return ambient + diffuse + specular;
            //return vec4(diffuse, 1.0f);
            //return vec4(texture(TINY_MatStd.diffuse, myUV).rgb, 1.0f);
        }

        void main()
        {
            vec3 normal = normalize(myNormal);
            vec3 view_dir = normalize(TINY_ViewPosition - myWorldPosition);

            myFinalColor = vec4(calcDirectionLight(TINY_LitDir, view_dir, normal), 1.0f);
            //myFinalColor = mix(calcDirectionLight(TINY_LitDir, viewDir, normal), reflection(-viewDir), 0.2f);
            //myFinalColor = reflection(-viewDir);
            //myFinalColor = refraction();
        }
    }
    #TINY_FS_END
}
#TINY_PASS_END
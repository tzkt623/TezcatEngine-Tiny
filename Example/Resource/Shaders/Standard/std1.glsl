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
        #include "tiny_vs_base"
        #include "tiny_vs_shadow"

        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aNormal;
        layout (location = 2) in vec2 aUV;
        layout (location = 3) in vec4 aColor;

        out vec4 myColor;
        out vec2 myUV;
        out vec3 myNormal;
        out vec3 myWorldPosition;
        out vec4 myLightPosition;

        void main()
        {
            vec4 position =  vec4(aPos, 1.0);
            gl_Position = TINY_MatrixP * TINY_MatrixV * TINY_MatrixM * position;

            myColor = aColor;
            myUV = aUV;
            myNormal = TINY_MatrixN * aNormal;
            myWorldPosition = vec3(TINY_MatrixM * position);
            myLightPosition = TINY_MatrixLit * vec4(myWorldPosition, 1.0f);
        }
    }
    #TINY_VS_END

    #TINY_FS_BEGIN
    {
        #include "tiny_fs_std_mat"
        #include "tiny_fs_light"
        #include "tiny_fs_camera"
        #include "tiny_fs_texture"
        #include "tiny_fs_function"

        in vec4 myColor;
        in vec2 myUV;
        in vec3 myNormal;
        in vec3 myWorldPosition;
        in vec4 myLightPosition;
        
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
            vec3 half_dir = normalize(viewDir + light_dir); 

            float NdL = dot(normal, light_dir);
            float NdH = dot(normal, half_dir);

            float diff = max(NdL, 0.0);
            float spec = pow(max(NdH, 0.0), TINY_MatStd.shininess);

            vec3 ambient = lit.ambient * texture(TINY_MatStd.diffuse, myUV).rgb;
            vec3 diffuse = lit.diffuse * diff * texture(TINY_MatStd.diffuse, myUV).rgb;
            vec3 specular = lit.specular * spec * texture(TINY_MatStd.specular, myUV).rrr;

            // shadow
            float shadow = calcShadow(myLightPosition, normal, light_dir, TINY_TexDepth);
            vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular));

            return lighting;
        }

        void main()
        {
            vec3 normal = normalize(myNormal);
            vec3 view_dir = normalize(TINY_ViewPosition - myWorldPosition);

            myFinalColor = vec4(calcDirectionLight(TINY_LitDir, view_dir, normal), 1.0f);
        }
    }
    #TINY_FS_END
}
#TINY_PASS_END
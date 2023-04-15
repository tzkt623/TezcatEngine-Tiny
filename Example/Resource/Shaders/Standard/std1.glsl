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
        layout (location = 2) in vec4 aColor;
        layout (location = 3) in vec2 aUV;

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
        #include "tiny_fs_struct"
        #include "tiny_fs_camera"
        #include "tiny_fs_texture"

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

        float calcShadow(vec4 lightPosition, vec3 normal, vec3 lightDir)
        {
            // 执行透视除法
            vec3 projCoords = lightPosition.xyz / lightPosition.w;
            // 变换到[0,1]的范围
            projCoords = projCoords * 0.5 + 0.5;

            //超出投影的远平面范围
            if(projCoords.z > 1.0)
            {
                return 0.0;
            }

            // 取得最近点的深度(使用[0,1]范围下的lightPosition当坐标)
            float closestDepth = texture(TINY_TexDepth, projCoords.xy).r; 
            // 取得当前片段在光源视角下的深度
            float currentDepth = projCoords.z;

            float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

            // 检查当前片段是否在阴影中
            //float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

            // PCF
            float shadow = 0.0;
            vec2 texelSize = 1.0 / textureSize(TINY_TexDepth, 0);
            for(int x = -1; x <= 1; ++x)
            {
                for(int y = -1; y <= 1; ++y)
                {
                    float pcfDepth = texture(TINY_TexDepth, projCoords.xy + vec2(x, y) * texelSize).r; 
                    shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
                }    
            }
            shadow /= 9.0;

            return shadow;
        }

        vec3 calcDirectionLight(LightDirection lit, vec3 viewDir, vec3 normal)
        {
            vec3 light_dir = normalize(-lit.direction);

            float diff = max(dot(normal, light_dir), 0.0);

            //vec3 reflect_dir = reflect(-light_dir, normal);
            vec3 halfwayDir = normalize(light_dir + viewDir); 
            //float spec = pow(max(dot(viewDir, reflect_dir), 0.0), TINY_MatStd.shininess);
            float spec = pow(max(dot(normal, halfwayDir), 0.0), TINY_MatStd.shininess);

            vec3 ambient = lit.ambient * texture(TINY_MatStd.diffuse, myUV).rgb;
            vec3 diffuse = lit.diffuse * diff * texture(TINY_MatStd.diffuse, myUV).rgb;
            vec3 specular = lit.specular * spec * texture(TINY_MatStd.specular, myUV).rrr;

            // shadow
            float shadow = calcShadow(myLightPosition, normal, light_dir);
            vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular));

            return lighting;
            //return ambient + diffuse + specular;
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
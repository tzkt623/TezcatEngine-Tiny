#TINY_HEAD_BEGIN
{
    str Name = Standard/PBRStd1;
}
#TINY_HEAD_END

#TINY_PASS_BEGIN
{
    #TINY_CFG_BEGIN
    {
        str Name = PBRStd1;
        int Version = 330;
        int OrderID = 50;
        str LightMode = Forward;
        str Queue = Opaque;
        str DepthTest = Less;
        bool ZWrite = true;
        str CullFace = Back;
        bool Blend = false;
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
            vec4 position = TINY_MatrixM * vec4(aPos, 1.0);

            myColor = aColor;
            myUV = aUV;
            myNormal = TINY_MatrixN * aNormal;
            myWorldPosition = vec3(position);

            gl_Position = TINY_MatrixP * TINY_MatrixV * position;
        }
    }
    #TINY_VS_END

    #TINY_FS_BEGIN
    {
        #include "tiny_fs_pbr_mat"
        #include "tiny_fs_pbr_func"
        #include "tiny_fs_camera"
        #include "tiny_fs_texture"
        #include "tiny_fs_light"

        in vec4 myColor;
        in vec2 myUV;
        in vec3 myNormal;
        in vec3 myWorldPosition;
        
        out vec4 myFinalColor;

        void main()
        {
            vec3 albedo = pow(texture(TINY_MatPBR_Std1.albedo2D, myUV).rgb, vec3(2.2));
            float metallic = texture(TINY_MatPBR_Std1.metallic2D, myUV).r;
            float roughness = texture(TINY_MatPBR_Std1.roughness2D, myUV).r;
            float ao = texture(TINY_MatPBR_Std1.ao2D, myUV).r;

            vec3 F0 = vec3(0.04); 
            F0 = mix(F0, albedo, metallic);

            vec3 N = normalize(myNormal);
            vec3 V = normalize(TINY_ViewPosition - myWorldPosition);
            vec3 R = reflect(-V, N); 

            //计算一个灯的光照
            vec3 Lo = vec3(0.0);
            {
                vec3 L = normalize(-TINY_LitDir.direction);
                vec3 H = normalize(V + L); 
                float cosTheta = max(dot(N, L), 0.0);
                vec3 radiance = TINY_LitDir.diffuse * cosTheta * 10;     

                float NDF = distributionGGX(N, H, roughness);
                float G = geometrySmith(N, V, L, roughness);
                vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

                vec3 kS = F;
                vec3 kD = vec3(1.0) - kS;
                kD *= 1.0 - metallic;

                vec3 nominator = NDF * G * F;
                float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; 
                vec3 specular = nominator / denominator;

                // add to outgoing radiance Lo
                float NdotL = max(dot(N, L), 0.0);                
                Lo += (kD * albedo / PI + specular) * radiance * NdotL; 
            }

            vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
    
            vec3 kS = F;
            vec3 kD = 1.0 - kS;
            kD *= 1.0 - metallic;	  
            vec3 irradiance = texture(TINY_TexIrradiance, N).rgb;
            vec3 diffuse = irradiance * albedo;

            const float MAX_REFLECTION_LOD = 4.0;
            vec3 prefilteredColor = textureLod(TINY_TexPrefilter, R,  roughness * MAX_REFLECTION_LOD).rgb;    
            vec2 brdf = texture(TINY_TexBRDFLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
            vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);

            vec3 ambient = (kD * diffuse + specular) * ao;
            vec3 color = ambient + Lo;

            color = color / (color + vec3(1.0));
            color = pow(color, vec3(1.0 / 2.2)); 

            myFinalColor = vec4(color, 1.0);
        }
    }
    #TINY_FS_END
}
#TINY_PASS_END
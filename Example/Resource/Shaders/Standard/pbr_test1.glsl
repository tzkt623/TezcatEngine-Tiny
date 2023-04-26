#TINY_HEAD_BEGIN
{
    str Name = Standard/PBRTest1;
}
#TINY_HEAD_END

#TINY_PASS_BEGIN
{
    #TINY_CFG_BEGIN
    {
        str Name = PBRTest1;
        int Version = 330;
        int OrderID = 50;
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
            vec3 albedo = TINY_MatPBR_Test.albedo;
            float metallic = TINY_MatPBR_Test.metallic;
            float roughness = TINY_MatPBR_Test.roughness;
            float ao = TINY_MatPBR_Test.ao, myUV;

            vec3 F0 = vec3(0.04); 
            F0 = mix(F0, albedo, metallic);

            vec3 N = normalize(myNormal);
            vec3 V = normalize(TINY_ViewPosition - myWorldPosition);


            //计算一个灯的光照
            vec3 Lo = vec3(0.0);
            vec3 L = normalize(-TINY_LitDir.direction);
            vec3 H = normalize(V + L); 
            float cosTheta = max(dot(N, L), 0.0);
            vec3 radiance = TINY_LitDir.diffuse * cosTheta * 10;     

            float NDF = DistributionGGX(N, H, roughness);
            float G = GeometrySmith(N, V, L, remapADirect(roughness));
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

            //计算漫反射颜色
            vec3 ambient = vec3(0.03) * albedo * ao;
            vec3 color = ambient + Lo;

            color = color / (color + vec3(1.0));
            color = pow(color, vec3(1.0 / 2.2)); 

            myFinalColor = vec4(color, 1.0);
        }
    }
    #TINY_FS_END
}
#TINY_PASS_END
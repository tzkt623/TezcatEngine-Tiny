#TINY_HEAD_BEGIN
{
    str Name = Unlit/EnvMakePrefilter;
}
#TINY_HEAD_END

#TINY_PASS_BEGIN
{
    #TINY_CFG_BEGIN
    {
        str Name = EnvMakePrefilter;
        int Version = 330;
        int OrderID = 50;
        str Queue = Prepare;
        str DepthTest = Off;
        bool ZWrite = true;
        str CullFace = Front;
    }
    #TINY_CFG_END

    #TINY_VS_BEGIN
    {
        #include "tiny_vs_base"

        layout (location = 0) in vec3 aPos;
        out vec3 myWorldPosition;

        void main()
        {
            myWorldPosition = aPos;
            gl_Position = TINY_MatrixP * TINY_MatrixV * vec4(aPos, 1.0);
        }
    }
    #TINY_VS_END

    #TINY_FS_BEGIN
    {
        #include "tiny_fs_texture"
        #include "tiny_fs_pbr_func"
        
        in vec3 myWorldPosition;
        out vec4 myFinalColor;

        uniform float roughness;

        void main()
        {       
            vec3 N = normalize(myWorldPosition);
            vec3 R = N;
            vec3 V = R;

            const uint SAMPLE_COUNT = 1024u;
            float totalWeight = 0.0;
            vec3 prefilteredColor = vec3(0.0);

            for(uint i = 0u; i < SAMPLE_COUNT; ++i)
            {
                // generates a sample vector that's biased towards the preferred alignment direction (importance sampling).
                vec2 Xi = hammersley(i, SAMPLE_COUNT);
                vec3 H = importanceSampleGGX(Xi, N, roughness);
                vec3 L = normalize(2.0 * dot(V, H) * H - V);

                float NdotL = max(dot(N, L), 0.0);
                if(NdotL > 0.0)
                {
                    // sample from the environment's mip level based on roughness/pdf
                    float D = distributionGGX(N, H, roughness);
                    float NdotH = max(dot(N, H), 0.0);
                    float HdotV = max(dot(H, V), 0.0);
                    float pdf = D * NdotH / (4.0 * HdotV) + 0.0001;

                    float resolution = 512.0; // resolution of source cubemap (per face)
                    float saTexel = 4.0 * PI / (6.0 * resolution * resolution);
                    float saSample = 1.0 / (float(SAMPLE_COUNT) * pdf + 0.0001);

                    float mipLevel = roughness == 0.0 ? 0.0 : 0.5 * log2(saSample / saTexel);
                    
                    prefilteredColor += textureLod(TINY_TexCube, L, mipLevel).rgb * NdotL;
                    totalWeight += NdotL;
                }
            }

            prefilteredColor = prefilteredColor / totalWeight;

            myFinalColor = vec4(prefilteredColor, 1.0);
        } 
    }
    #TINY_FS_END
}
#TINY_PASS_END
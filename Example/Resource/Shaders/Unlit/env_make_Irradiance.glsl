#TINY_HEAD_BEGIN
{
    str Name = Unlit/EnvMakeIrradiance;
}
#TINY_HEAD_END

#TINY_PASS_BEGIN
{
    #TINY_CFG_BEGIN
    {
        str Name = EnvMakeIrradiance;
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

        void main()
        {		
            // The world vector acts as the normal of a tangent surface
            // from the origin, aligned to WorldPos. Given this normal, calculate all
            // incoming radiance of the environment. The result of this radiance
            // is the radiance of light coming from -Normal direction, which is what
            // we use in the PBR shader to sample irradiance.
            vec3 N = normalize(myWorldPosition);

            vec3 irradiance = vec3(0.0);   
            
            // tangent space calculation from origin point
            vec3 up = vec3(0.0, 1.0, 0.0);
            vec3 right = normalize(cross(up, N));
            up = normalize(cross(N, right));
            
            float sampleDelta = 0.025;
            float nrSamples = 0.0;
            for(float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
            {
                for(float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
                {
                    // spherical to cartesian (in tangent space)
                    vec3 tangentSample = vec3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta));
                    // tangent space to world
                    vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * N; 

                    irradiance += texture(TINY_TexCube, sampleVec).rgb * cos(theta) * sin(theta);
                    nrSamples++;
                }
            }
            irradiance = PI * irradiance * (1.0 / float(nrSamples));
            
            myFinalColor = vec4(irradiance, 1.0);
        }
    }
    #TINY_FS_END
}
#TINY_PASS_END
#TINY_HEAD_BEGIN
{
    str Name = Unlit/EnvMakeCube;
}
#TINY_HEAD_END

#TINY_PASS_BEGIN
{
    #TINY_CFG_BEGIN
    {
        str Name = EnvMakeCube;
        int Version = 330;
        int OrderID = 50;
        str Queue = Opaque;
        str DepthTest = LessEqual;
        bool ZWrite = true;
        str CullFace = Off;
    }
    #TINY_CFG_END

    #TINY_VS_BEGIN
    {
        #include "tiny_vs_base"

        layout (location = 0) in vec3 aPos;
        out vec3 myUV;

        void main()
        {
            myUV = aPos;
            gl_Position = TINY_MatrixP * TINY_MatrixV * vec4(aPos, 1.0);
        }
    }
    #TINY_VS_END

    #TINY_FS_BEGIN
    {
        #include "tiny_fs_texture"
        
        in vec3 myUV;
        out vec4 myFinalColor;

        const vec2 invAtan = vec2(0.1591, 0.3183);
        vec2 sampleSphericalMap(vec3 v)
        {
            vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
            uv *= invAtan;
            uv += 0.5;
            return uv;
        }

        void main()
        {
            vec2 uv = sampleSphericalMap(normalize(myUV));
            vec3 color = texture(TINY_TexColor, uv).rgb;

            myFinalColor = vec4(color, 1.0f);
        }
    }
    #TINY_FS_END
}
#TINY_PASS_END
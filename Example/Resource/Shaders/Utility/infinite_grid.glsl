#TINY_HEAD_BEGIN
{
    str Name = Utility/InfiniteGrid;
}
#TINY_HEAD_END

#TINY_PASS_BEGIN
{
    #TINY_CFG_BEGIN
    {
        str Name = InfiniteGrid;
        int Version = 330;
        int OrderID = 60;
        str Queue = Background;
        str DepthTest = Off;
        bool ZWrite = true;
        bool Blend = true;
        str BlendSrc = SrcA;
        str BlendTar = 1-SrcA;
        str CullFace = Off;
    }
    #TINY_CFG_END

    #TINY_VS_BEGIN
    {
        #include "tiny_vs_base"

        layout (location = 0) in vec3 aPos;

        out vec3 nearPoint;
        out vec3 farPoint;

        out mat4 fragView;
        out mat4 fragProj;

        vec3 UnprojectPoint(float x, float y, float z, mat4 view, mat4 projection)
        {
            mat4 viewInv = inverse(view);
            mat4 projInv = inverse(projection);
            vec4 unprojectedPoint =  viewInv * projInv * vec4(x, y, z, 1.0);
            return unprojectedPoint.xyz / unprojectedPoint.w;
        }

        void main()
        {
            nearPoint = UnprojectPoint(aPos.x, aPos.y, 0.0, TINY_MatrixV, TINY_MatrixP);
            farPoint = UnprojectPoint(aPos.x, aPos.y, 1.0, TINY_MatrixV, TINY_MatrixP);
            fragView = TINY_MatrixV;
            fragProj = TINY_MatrixP;

            gl_Position = vec4(aPos, 1.0);
        }
    }
    #TINY_VS_END

    #TINY_FS_BEGIN
    {
        #include "tiny_fs_camera"

        in vec3 nearPoint;
        in vec3 farPoint;
        in mat4 fragView;
        in mat4 fragProj;

        out vec4 myGirdColor;

        vec4 grid(vec3 fragPos3D, float scale)
        {
            vec2 coord = fragPos3D.xz * scale; // use the scale variable to set the distance between the lines
            vec2 derivative = fwidth(coord);
            vec2 grid = abs(fract(coord - 0.5) - 0.5) / derivative;
            float line = min(grid.x, grid.y);
            float minimumz = min(derivative.y, 1);
            float minimumx = min(derivative.x, 1);
            vec4 color = vec4(0.2, 0.2, 0.2, 1.0 - min(line, 1.0));
            // z axis
            if(fragPos3D.x > -0.1 * minimumx && fragPos3D.x < 0.1 * minimumx)
                color.z = 1.0;
            // x axis
            if(fragPos3D.z > -0.1 * minimumz && fragPos3D.z < 0.1 * minimumz)
                color.x = 1.0;
            return color;
        }

        float computeDepth(vec3 pos)
        {
            vec4 clip_space_pos = fragProj * fragView * vec4(pos.xyz, 1.0);
            return (clip_space_pos.z / clip_space_pos.w);
        }

        float computeLinearDepth(vec3 pos)
        {
            vec4 clip_space_pos = fragProj * fragView * vec4(pos.xyz, 1.0);
            float clip_space_depth = (clip_space_pos.z / clip_space_pos.w) * 2.0 - 1.0; // put back between -1 and 1
            float linearDepth = (2.0 * TINY_ViewNearFar.x * TINY_ViewNearFar.y) / (TINY_ViewNearFar.y + TINY_ViewNearFar.x - clip_space_depth * (TINY_ViewNearFar.y - TINY_ViewNearFar.x)); // get linear value between 0.01 and 100
            return linearDepth / TINY_ViewNearFar.y; // normalize
        }

        void main()
        {
            float t = -nearPoint.y / (farPoint.y - nearPoint.y);
            vec3 fragPos3D = nearPoint + t * (farPoint - nearPoint);
            gl_FragDepth = computeDepth(fragPos3D);

            float linearDepth = computeLinearDepth(fragPos3D);
            float fading = max(0, (0.5 - linearDepth));

            myGirdColor = (grid(fragPos3D, 0.01) + grid(fragPos3D, 0.01)) * float(t > 0);
            //myGirdColor.a = fading;
        }
    }
    #TINY_FS_END
}
#TINY_PASS_END
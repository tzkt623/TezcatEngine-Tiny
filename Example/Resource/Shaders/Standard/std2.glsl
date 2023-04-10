#TINY_HEAD_BEGIN
{
    str Name = Standard/Std2;
}
#TINY_HEAD_END

#TINY_PASS_BEGIN
{
    #TINY_CFG_BEGIN
    {
        str Name = Std2/Base;
        int Version = 330;
        int OrderID = 50;
        str Queue = Opaque;
        bool ZWrite = true;
        str CullFace = Back;
        bool Blend = false;
        str BlendSrc = 1;
        str BlendTar = 1-TarA;
    }
    #TINY_CFG_END

    #TINY_TAG_BEGIN
    {
        
    }
    #TINY_TAG_END

    #TINY_CFG_BEGIN
    {
        str 
        str Queue = Opaque;
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
        struct TINY_Mateial_Std
        {
            sampler2D diffuse;
            sampler2D specular;
            float shininess;
        };

        struct TINY_Light_Direction
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

        uniform TINY_Mateial_Std TINY_MatStd;
        uniform TINY_Light_Direction TINY_LightDir;
        uniform vec3 TINY_ViewPosition;
    
        out vec4 myFinalColor;

        vec3 calculateDirectionLight(TINY_Light_Direction light, vec3 normal, vec3 viewDir)
        {
            vec3 light_dir = normalize(-light.direction);

            vec3 ambient = light.ambient * texture(TINY_MatStd.diffuse, myUV).rgb;

            float diff = max(dot(normal, light_dir), 0.0);
            vec3 diffuse = light.diffuse * diff * texture(TINY_MatStd.diffuse, myUV).rgb;

            //blinn-phong
            vec3 halfway = normalize(light_dir + viewDir);
            float spec = pow(max(dot(normal, halfway), 0.0), TINY_MatStd.shininess);
            //phong
            //vec3 reflect_dir = reflect(-light_dir, normal);
            //float spec = pow(max(dot(viewDir, reflect_dir), 0.0), TINY_MatStd.shininess);

            vec3 specular = light.specular * spec * texture(TINY_MatStd.specular, myUV).rgb;

            return ambient + diffuse + specular;
        }

        void main()
        {
            vec3 normal = normalize(myNormal);
            vec3 viewDir = normalize(TINY_ViewPosition - myWorldPosition);

            vec3 dir_light = calculateDirectionLight(TINY_LightDir, normal, viewDir);
            myFinalColor = vec4(dir_light, 1.0f);
        }
    }
    #TINY_FS_END
}
#TINY_PASS_END

#TINY_PASS_BEGIN
{
    #TINY_CFG_BEGIN
    {
        str Name = Std2/Add;
        int Version = 330;
        int OrderID = 0;
        str Queue = Transparent;
        str DepthTest = off;
        bool ZWrite = false;
        str CullFace = Back;
        bool Blend = true;
        str BlendSrc = 1;
        str BlendTar = 1;
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
        struct TINY_Mateial_Std
        {
            sampler2D diffuse;
            sampler2D specular;
            float shininess;
        };

        struct TINY_Light_Point
        {
            vec3 position;

            vec3 ambient;
            vec3 diffuse;
            vec3 specular;

            vec3 config;
            //float constant;
            //float linear;
            //float quadratic;
        };

        in vec4 myColor;
        in vec2 myUV;
        in vec3 myNormal;
        in vec3 myWorldPosition;

        uniform TINY_Mateial_Std TINY_MatStd;
        uniform TINY_Light_Point TINY_LightPot;
        uniform vec3 TINY_ViewPosition;

        out vec4 myFinalColor;

        vec3 calculatePoint(TINY_Light_Point light, vec3 normal, vec3 viewDir)
        {
            vec3 light_dir = normalize(light.position - myWorldPosition);

            vec3 ambient = light.ambient * texture(TINY_MatStd.diffuse, myUV).rgb;

            float diff = max(dot(normal, light_dir), 0.0);
            vec3 diffuse = light.diffuse * diff * texture(TINY_MatStd.diffuse, myUV).rgb;

            vec3 reflect_dir = reflect(-light_dir, normal);
            float spec = pow(max(dot(viewDir, reflect_dir), 0.0), TINY_MatStd.shininess);
            vec3 specular = light.specular * spec * texture(TINY_MatStd.specular, myUV).rgb;

            return ambient + diffuse + specular;
        }

        void main()
        {
            vec3 normal = normalize(myNormal);
            vec3 viewDir = normalize(TINY_ViewPosition - myWorldPosition);

            vec3 point_light = calculatePoint(TINY_LightPot, normal, viewDir);
            myFinalColor = vec4(point_light, 1.0f);
        }
    }
    #TINY_FS_END
}
#TINY_PASS_END
# TezcatEngine-Tiny

# **引擎二周目进行中**

![示例](https://github.com/tzkt623/TezcatEngine-Tiny/blob/main/Resource/Image/logo.jpg?raw=true)
## **代码结构**

因为我用unity比较多,就模仿了他的结构,现在的结构是这样
```c
auto elden_ring1_material = new Material("Unlit/Texture");
elden_ring1_material->addTexture2D(TinyParameter::TexColor, "../Resource/Image/eldenring1.jpg");

auto elden_ring1 = new GameObject();
auto mre1 = elden_ring1->addComponent<MeshRenderer>();
mre1->setMaterial(elden_ring1_material);
mre1->setMesh("Square");
elden_ring1->getTransform()->setPosition(glm::vec3(0.0f, 0.0f, -960.0f));
elden_ring1->getTransform()->setRotation(glm::vec3(0.0f, 0.0f, 0.0f));
elden_ring1->getTransform()->setScale(glm::vec3(1920.0f, 1080.0f, 1));
this->addGameObject(elden_ring1);

//---------------------------------------------------------

auto elden_ring2_material = new Material("Unlit/Texture");
elden_ring2_material->addTexture2D(TinyParameter::TexColor, "../Resource/Image/eldenring2.jpg");

auto elden_ring2 = new GameObject();
auto mre2 = elden_ring2->addComponent<MeshRenderer>();
mre2->setMaterial(elden_ring2_material);
mre2->setMesh("Square");

elden_ring2->getTransform()->setPosition(glm::vec3(0.0f, 0.0f, 960.0f));
elden_ring2->getTransform()->setRotation(glm::vec3(0.0f, -180.0f, 0.0f));
elden_ring2->getTransform()->setScale(glm::vec3(1920.0f, 1080.0f, 1));
this->addGameObject(elden_ring2);
```
## **材质结构**

### **默认值 default**
除了[`int Version`]为必须值,其他值均为拥有默认值的可选参数

The[`int Version`] should be setted.The other params You can set as your wish.

### **混合 Blend**
混合参数 BlendFunc

```c
"0",			ZERO
"1",			ONE
"Src",			SRC_COLOR
"1-Src",		ONE_MINUS_SRC_COLOR
"Tar",			DST_COLOR
"1-Tar",		ONE_MINUS_DST_COLOR
"SrcA",		    SRC_ALPHA
"1-SrcA",		ONE_MINUS_SRC_ALPHA
"TarA",		    DST_ALPHA
"1-TarA",		ONE_MINUS_DST_ALPHA
"Const",		CONSTANT_COLOR
"1-Const",		ONE_MINUS_CONSTANT_COLOR
"ConstA",		CONSTANT_ALPHA
"1-ConstA",	    ONE_MINUS_CONSTANT_ALPHA
```

启用混合 EnableBlend

```c
bool Blend = true;
str BlendSrc = 1;
str BlendTar = 1-TarA;
```

### **剔除 Cull**
剔除参数 Cullface
```c
"Off",		disable
"Front",	FRONT
"Back",	    BACK
"All",		FRONT_AND_BACK
```

启用剔除 EnableCullface

```c
str CullFace = Back;
```

### **深度测试 DepthTest**
启用 Enable
```c
bool ZWrite = true;
```


### **示例 example**
```c
#TINY_HEAD_BEGIN
{
    str Name = Standard/Std1;
}
#TINY_HEAD_END

#TINY_PASS_BEGIN
{
    #TINY_CFG_BEGIN
    {
        int Version = 330;
        int OrderID = 0;
        str Queue = Opaque;
        bool Lighting = true;
        bool ZWrite = true;
        str CullFace = Back;
        bool Blend = false;
        str BlendSrc = 1;
        str BlendTar = 1-TarA;
    }
    #TINY_CFG_END

    #TINY_VS_BEGIN
    {
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aNormal;
        layout (location = 2) in vec4 aColor;
        layout (location = 3) in vec2 aUV;

        uniform mat4 TINY_MatP;
        uniform mat4 TINY_MatV;
        uniform mat4 TINY_MatM;
        uniform mat3 TINY_MatN;

        out vec4 myColor;
        out vec2 myUV;
        out vec3 myNormal;
        out vec3 myWorldPosition;

        void main()
        {
            vec4 position =  vec4(aPos, 1.0);
            gl_Position = TINY_MatP * TINY_MatV * TINY_MatM * position;
            
            myColor = aColor;
            myUV = aUV;
            myNormal = TINY_MatN * aNormal;
            myWorldPosition = vec3(TINY_MatM * position);
        }
    }
    #TINY_VS_END

    #TINY_FS_BEGIN
    {
        in vec4 myColor;
        in vec2 myUV;
        in vec3 myNormal;
        in vec3 myWorldPosition;

        uniform float TINY_AmbientStrength = 0.1f;
        uniform vec3 TINY_LightPosition = vec3(0.0f, 0.0f, 0.0f);
        uniform vec3 TINY_LightColor = vec3(1.0f, 1.0f, 1.0f);
        uniform vec3 TINY_ViewPosition;
        uniform sampler2D TINY_TexColor;

        out vec4 myFinalColor;

        float specularStrength = 0.5;

        void main()
        {
            vec3 ambient = TINY_AmbientStrength * TINY_LightColor;

            vec3 normal = normalize(myNormal);
            vec3 lightDir = normalize(TINY_LightPosition - myWorldPosition);
            float diff = max(dot(normal, lightDir), 0.0);
            vec3 diffuse = diff * TINY_LightColor;

            vec3 viewDir = normalize(TINY_ViewPosition - myWorldPosition);
            vec3 reflectDir = reflect(-lightDir, normal);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
            vec3 specular = specularStrength * spec * TINY_LightColor;

            myFinalColor = vec4(ambient + diffuse + specular, 1.0f) * myColor * texture(TINY_TexColor, myUV);
        }
    }
    #TINY_FS_END
}
#TINY_PASS_END
```
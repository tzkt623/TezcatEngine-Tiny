# TezcatEngine-Tiny

# **引擎二周目进行中**

![示例](https://github.com/tzkt623/TezcatEngine-Tiny/blob/main/Resource/Image/logo1.gif?raw=true)
![示例](https://github.com/tzkt623/TezcatEngine-Tiny/blob/main/Resource/Image/logo2.gif?raw=true)

注意!本引擎使用C++20版本

Notice!Tiny Use The C++20 Ver.

具体使用方法请看Example.

Please check the Example project to know more info.

## **代码结构 Code**

因为我用unity比较多,就模仿了他的结构,现在的结构是这样

i am a Unity user, so Tiny look like this
```cpp
auto elden_ring2 = new GameObject();
elden_ring2->addComponent<Transform>();
elden_ring2->getTransform()->setPosition(glm::vec3(0.0f, 0.0f, 960.0f));
elden_ring2->getTransform()->setRotation(glm::vec3(0.0f, -180.0f, 0.0f));
elden_ring2->getTransform()->setScale(glm::vec3(1920.0f, 1080.0f, 1));

auto mre2 = elden_ring2->addComponent<MeshRenderer>();
auto elden_ring2_material = new Material("Unlit/Texture");
elden_ring2_material->addUniform<UniformTex2D>(ShaderParam::TexColor, "../Resource/Image/eldenring2.jpg");
mre2->setMaterial(elden_ring2_material);
mre2->setMesh("Square");

//---------------------------------------------------------

auto plane = new GameObject();
plane->addComponent<Transform>();
plane->getTransform()->setPosition(glm::vec3(0.0f, -20.0f, 0.0f));
plane->getTransform()->setRotation(glm::vec3(-90.0f, 0.0f, 0.0f));
plane->getTransform()->setScale(glm::vec3(10.0f, 10.0f, 1));

auto mr = plane->addComponent<MeshRenderer>();
auto plane_material = new Material("Standard/Std2");
plane_material->addUniform<UniformTex2D>(ShaderParam::StdMaterial::Diffuse, "../Resource/Image/container.png");
plane_material->addUniform<UniformTex2D>(ShaderParam::StdMaterial::Specular, "../Resource/Image/container_specular.png");
plane_material->addUniform<UniformF>(ShaderParam::StdMaterial::Shininess, 64.0f);
mr->setMaterial(plane_material);
mr->setMesh("Square");
```


## **材质结构 Material**

目前Shader构建器可以自动解析出所有非数组的Uniform变量.

ShaderBuilder can auto scan all GLSL Uniform value except array type.

目前Tiny的内建变量如下

Tiny Current Buildin Uniform Values

```cpp
static UniformID Empty;

static UniformID MatrixP;
static UniformID MatrixV;
static UniformID MatrixM;
static UniformID MatrixN;

static UniformID VertexColor;
static UniformID TexColor;

static UniformID LightPosition;
static UniformID LightColor;
static UniformID ViewPosition;
static UniformID AmbientStrength;
static UniformID ModeSpecular;

struct LightDirection
{
    static UniformID Direction;
    static UniformID Ambient;
    static UniformID Diffuse;
    static UniformID Specular;
};

struct StdMaterial
{
    static UniformID Diffuse;
    static UniformID Specular;
    static UniformID Shininess;
};
```

**目前内建材质变量类型有**(后续会慢慢添加)

Current Buildin Material Values

|    TinyType     |  CommonType   |
| :-------------: | :-----------: |
|  UniformI[1-4]  |   int[1-4]    |
|  UniformF[1-4]  |  float[1-4]   |
| UniformMat[3-4] | glm::mat[3-4] |
|  UniformTex2D   |   Texture2D   |

给一个object添加一个材质之后一定要记得添加材质对应的参数,贴图,数值等等

notice! add uniform value to your material for the gameobject.

```cpp
auto material = new Material("Standard/Std1");
material->addUniform<UniformTex2D>(ShaderParam::TexColor, "../Resource/Image/dragon.jpg");
material->addUniform<UniformF>(ShaderParam::ModeSpecular, 64.0f);
```

### **默认值 DefaultValue**
除了[`int Version`]为必须值,其他值均为拥有默认值的可选参数

The[`int Version`] should be setted.The other params You can set as your wish.

### **管线位置 PipelineQueue**
Forward管线位置
Forward Pipeline Position
```cpp
"Background"    Forward::Background
"Opaque"        Forward::Geometry
"Alpha"         Forward::AlphaTest
"OpaqueLast"    Forward::OpaqueLast
"Transparent"   Forward::Transparent
"Overlay"       Forward::Overlay
```

### **混合 Blend**
混合参数 BlendFunc

```cpp
"0"         ZERO
"1"         ONE
"Src"       SRC_COLOR
"1-Src"     ONE_MINUS_SRC_COLOR
"Tar"       DST_COLOR
"1-Tar"     ONE_MINUS_DST_COLOR
"SrcA"	    SRC_ALPHA
"1-SrcA"    ONE_MINUS_SRC_ALPHA
"TarA"      DST_ALPHA
"1-TarA"    ONE_MINUS_DST_ALPHA
"Const"     CONSTANT_COLOR
"1-Const"   ONE_MINUS_CONSTANT_COLOR
"ConstA"    CONSTANT_ALPHA
"1-ConstA"  ONE_MINUS_CONSTANT_ALPHA
```

启用混合 EnableBlend

```c
bool Blend = true;
str BlendSrc = 1;
str BlendTar = 1-TarA;
```

关闭混合 DisableBlend

```c
bool Blend = false;
```

### **表面剔除 Cullface**
剔除参数 Cullface
```c
"Off"       Disable
"Front"     FRONT
"Back"      BACK
"All"       FRONT_AND_BACK
```

启用剔除 EnableCullface
```c
str CullFace = Back;
```

关闭剔除
```c
str CullFace = Off;
```

### **深度测试 DepthTest**
测试参数
```c
"Off"               Off
"Always"            Always
"Never"             Never
"Less"              Less
"LessEqual"         LessEqual
"Greater"           Greater
"GreaterEqual"      GreaterEqual
"Equal"             Equal
"NotEqual"          NotEqual
```

启用 Enable
```c
str DepthTest = Less;
```
关闭 Disable
```c
str DepthTest = Off;
```
### **深度写入 ZWrite**
只有在深度测试启用时才有用 Only work when DepthTest is Enabled

启用 Enable
```c
bool ZWrite = true;
```

关闭 Disable
```c
bool ZWrite = false;
```

### **示例 example**
```glsl
#TINY_HEAD_BEGIN
{
    str Name = Standard/Std2;
}
#TINY_HEAD_END

#TINY_PASS_BEGIN
{
    #TINY_CFG_BEGIN
    {
        str Name = Std2;
        int Version = 330;
        int OrderID = 0;
        str Queue = Opaque;
        bool Lighting = true;
        str DepthTest = Less;
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
            float diff = max(dot(normal, light_dir), 0.0);
            vec3 reflect_dir = reflect(-light_dir, normal);
            float spec = pow(max(dot(viewDir, reflect_dir), 0.0), TINY_MatStd.shininess);

            vec3 ambient = light.ambient * texture(TINY_MatStd.diffuse, myUV).rgb;
            vec3 diffuse = light.diffuse * diff * texture(TINY_MatStd.diffuse, myUV).rgb;
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
```
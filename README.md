# TezcatEngine-Tiny

## **引擎二周目进行中**

![示例](https://github.com/tzkt623/TezcatEngine-Tiny/blob/main/Resource/Image/logo1.jpg?raw=true)
![示例](https://github.com/tzkt623/TezcatEngine-Tiny/blob/main/Resource/Image/logo2.jpg?raw=true)

注意!本引擎使用C++20版本

Notice!Tiny Use The C++20 Ver.

## **代码结构 Code**

因为我用unity比较多,就模仿了他的结构,现在的结构是这样

I am a Unity user, so i image that and make Tiny looks like this

```cpp
auto go = new GameObject("World1_Camera");
auto camera = go->addComponent<Camera>(true);
camera->setPerspective(60.0f, 0.1f, 2000.0f);
camera->setCullLayer(0);

go->addComponent<Transform>();
go->getTransform()->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
go->getTransform()->setParent(controller_go->getTransform());

//-------------------------------------
go = new GameObject("Skybox1");
go->addComponent<Transform>();

auto skybox = go->addComponent<Skybox>();
auto material = new Material("Unlit/Skybox");
material->addUniform<UniformTexCube>(ShaderParam::TexCube, "skybox_2");
skybox->setMaterial(material);

//---------------------------------------------------------
auto world2 = new GameObject("World2_Gate");
auto tran = world2->addComponent<Transform>();
tran->setPosition(glm::vec3(-300.0f, 0.0f, 0.0f));
tran->setRotation(glm::vec3(0.0f, 90.0f, 0.0f));
tran->setScale(glm::vec3(gateWidth, gateHigh, 1.0f));

auto mr1 = world2->addComponent<MeshRenderer>();
auto world2_material = new Material("Unlit/Texture");
world2_material->addUniform<UniformTex2D>(ShaderParam::TexColor, "RB_World2");
mr1->setMaterial(world2_material);
mr1->setMesh("Square");
```

## **创建游戏对象 Create GameObjects**

- 创建相机
  Create a Camera

```cpp
//gameobject will auto load into current scene
auto go = new GameObject("World1_Camera");
//attach a Camera component
auto camera = go->addComponent<Camera>(true);
camera->setPerspective(60.0f, 0.1f, 2000.0f);
//set culling layer, other layers will not render
camera->setCullLayer(0);
//attach a Transform component
go->addComponent<Transform>();
go->getTransform()->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
go->getTransform()->setParent(controller_go->getTransform());
```

- 创建天空盒
  Create a Skybox

```cpp
auto go = new GameObject("Skybox1");
//set layer, a camera just render objects with thesame layer id
go->setLayerMaskIndex(1);
go->addComponent<Transform>();

//attach a Skybox component
auto skybox = go->addComponent<Skybox>();
//load Material with ShaderName(Buildin Shader)
auto material = new Material("Unlit/Skybox");
//add a UniformValue to auto update value in shader
//note that [skybox_2] is a [SkyboxName] auto loaded in [TextureManager], not the imagefile(.jpg.png....) name
material->addUniform<UniformTexCube>(ShaderParam::TexCube, "skybox_2");
skybox->setMaterial(material);
```

- 创建一个游戏物体(传送门)
  Create a GameObject(JumpGate)

```cpp
//default layer is 0
auto world2 = new GameObject("World2_Gate");
auto tran = world2->addComponent<Transform>();
tran->setPosition(glm::vec3(-300.0f, 0.0f, 0.0f));
tran->setRotation(glm::vec3(0.0f, 90.0f, 0.0f));
tran->setScale(glm::vec3(gateWidth, gateHigh, 1.0f));

auto mr1 = world2->addComponent<MeshRenderer>();
auto world2_material = new Material("Unlit/Texture");
//note that [RB_World2] is a [FrameBuffer] created in [TextureManager] by yourself
world2_material->addUniform<UniformTex2D>(ShaderParam::TexColor, "RB_World2");
mr1->setMaterial(world2_material);
mr1->setMesh("Square");
```

- 创建一个帧缓冲
  Create a FrameBuffer

```cpp
auto fb = FrameBufferMgr::getInstance()->create(
    //Name for find
    "FB_World1",
    //width, high
    Engine::getScreenWidth(), Engine::getScreenHeight(),
    {
        //Create a ColorBuffer Setting
        TextureBufferInfo(
            //a Name for find(manager flag is true like this)
            "RB_World1"
            //Buffer Type(FrameBuffer Component)
            , TextureBufferType::ColorComponent
            //Internal Format
            , TextureChannel::RGBA
            //Format
            , TextureChannel::RGBA
            //Data Type
            , DataType::UByte
            //a flag for cache this texutreBuffer in [TextureManager] to find
            , true),
        //Create a write-only Buffer Setting for DepthAndStencil
        TextureBufferInfo(
            //a Name for find(manager flag is false so noneffective)
            "DS_World1"
            //Buffer Type(FrameBuffer Component)
            , TextureBufferType::DepthStencilComponent
            //Internal Format
            , TextureChannel::Depth24_Stencil8)
    });

//let camera render objects to this framebuffer
camera->setFrameBuffer(fb);

//also you can find this framebuffer like
fb = FrameBufferMgr::getInstance()->find("FB_World1");

//set nullptr will switch render to mainframe
camera->setFrameBuffer(nullptr);
```

**具体使用方法请看Example.**

**Please check the Example project to get more infos.**

## **资源管理 Resouce Manage**

注意! .exe文件必须和资源文件夹处于同一目录下

Attention! The .exe file must be in the same directory as the resource folder

1. 继承并实现`ResourceLoader`类
   Inherit and implement the `ResourceLoader` class

    ```cpp
    class MyResourceLoader : public ResourceLoader
    {
    public:
        MyResourceLoader();

        void prepareEngine(Engine* engine) override; 
        void prepareResource(Engine* engine) override;
        void prepareGame(Engine* engine) override;
        void initYourShaderParam() override;
    };
    ```

    `prepareEngine`,`prepareResource`,`prepareGame`三个方法会按此顺序依次执行
    `prepareEngine`,`prepareResource`,`prepareGame`are invoked in this order

2. 设置自己的资源文件夹名称,程序名称,屏幕大小
   Set your **ResourceFolder Name, ProgramName, ScreenSize**

    ```cpp
    void MyResourceLoader::prepareEngine(Engine* engine)
    {
        ResourceLoader::prepareEngine(engine);
        mResourceFolderName = "Resource";
        mGameName = u8"YesIndeed,玩上老头环了!!!!!";
        mWindowWidth = 1920;
        mWindowHeight = 1080;
    }
    ```

3. 加载资源文件
   Load resource files

    ```cpp
    void MyResourceLoader::prepareResource(Engine* engine)
    {
        ResourceLoader::prepareResource(engine);
        //设置图片文件夹名称自动加载所有图片文件
        //注意,不同文件夹下面的图片文件也不能重名
        //Set ImageFolder to auto load all images
        //Note that the image files under different folders also must not have the same name
        TextureMgr::getInstance()->loadResource("/Image");
    }
    ```

4. 加载第一个Scene
   Load first Scene

    ```cpp
    void MyResourceLoader::prepareGame(Engine* engine)
    {
        ResourceLoader::prepareGame(engine);
        SG<SceneManager>::getInstance()->prepareScene(new MyScene("MyScene"));
        SG<SceneManager>::getInstance()->pushScene("MyScene");
    }
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
|:---------------:|:-------------:|
|  UniformI[1-4]  |   int[1-4]    |
|  UniformF[1-4]  |  float[1-4]   |
| UniformMat[3-4] | glm::mat[3-4] |
|  UniformTex2D   |   Texture2D   |
| UniformTexCube  |  TextureCube  |

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

Forward管线位置 ForwardPipeline Position

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
"SrcA"      SRC_ALPHA
"1-SrcA"    ONE_MINUS_SRC_ALPHA
"TarA"      DST_ALPHA
"1-TarA"    ONE_MINUS_DST_ALPHA
"Const"     CONSTANT_COLOR
"1-Const"   ONE_MINUS_CONSTANT_COLOR
"ConstA"    CONSTANT_ALPHA
"1-ConstA"  ONE_MINUS_CONSTANT_ALPHA
```

启用混合 EnableBlend

```cpp
bool Blend = true;
str BlendSrc = 1;
str BlendTar = 1-TarA;
```

关闭混合 DisableBlend

```cpp
bool Blend = false;
```

### **表面剔除 Cullface**

剔除参数 Cullface

```cpp
"Off"       Disable
"Front"     FRONT
"Back"      BACK
"All"       FRONT_AND_BACK
```

启用剔除 EnableCullface

```cpp
str CullFace = Back;
```

关闭剔除

```cpp
str CullFace = Off;
```

### **深度测试 DepthTest**

测试参数

```cpp
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

```cpp
str DepthTest = Less;
```

关闭 Disable

```cpp
str DepthTest = Off;
```

### **深度写入 ZWrite**

只有在深度测试启用时才有用 Only work when DepthTest is Enabled

启用 Enable

```cpp
bool ZWrite = true;
```

关闭 Disable

```cpp
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

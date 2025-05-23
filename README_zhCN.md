# TezcatEngine-Tiny

![Static Badge](https://img.shields.io/badge/License-GPL3.0-brightgreen)

![Static Badge](https://img.shields.io/badge/Language-C%2B%2B-blue)
![Static Badge](https://img.shields.io/badge/C%2B%2B-20-gold)

![Static Badge](https://img.shields.io/badge/IDE-VS2022-red)

## **README**

- en [English](README.md)
- zh_CN [简体中文](README_zhCN.md)

------------------------------------------------

## **引擎二周目进行中**

![Image](https://github.com/user-attachments/assets/bfecae67-9533-4ffa-8bc8-179ffbba2b2b)

![Image](https://github.com/user-attachments/assets/9375aac2-c786-418e-ab1a-3e38430c4655)

![logo1](https://github.com/tzkt623/TezcatEngine-Tiny/assets/6510903/1faf72c8-36e8-4bb3-9e40-2b87a0656dfc)

![logo2](https://github.com/tzkt623/TezcatEngine-Tiny/assets/6510903/0a9e3960-eae2-4ea7-8d4c-149b9a3a0ef4)

-------------------------------------------------

## **Update**

- [ ] 正在添加一个引擎内部的名称系统
- [x] 添加管线查看器
- [x] 分离了编辑器相机和游戏相机
- [x] 重写了资源加载方式,不再默认保存资源文件夹,需要输入全路径
- [x] 现在可以拖拽模型到总览中进行加载了
- [x] 现在可以拖拽图片到材质中进行替换
- [x] 现在可以选中场景中的单位,总览会进行自动选择
- [x] 主相机会自动启动单位选择系统
- [x] 添加了UnfiormBuffer
- [x] 一些Shader内建变量被UniformBuffer代替
- [x] 全新的渲染流程架构,更加的合理好用
- [x] 可以加载和显示Shader数组对象了!
- [x] 约束回归!在所有Uniform对象里设置约束信息以显示在Editor中.

## **Issue**

- [ ] PBR效果被我改坏了
- [ ] Shader编辑器有点问题
- [ ] 暂时取消多线程模式
- [ ] 内存查看器有空对象读取bug

-------------------------------------------

## To Do

Engine

- [x] Basic PBR
- [x] Octree
- [x] Commad Based Rendering
- [x] Basic GL Shader Parser
- [x] Basic ShadowMap
- [x] Basic Memory Manager
- [x] Basic Multi-Thread Engine
- [x] Basic Resource Manager
- [ ] Camera Culling
- [ ] Multi-Light Support
- [x] Model Load Support
- [ ] Transparent Sort
- [ ] Multi-Thread Rendering(Vulkan)

Editor

- [x] Basic Shader Editor
- [x] Basic Lighting Manager
- [x] Basic Memory Viewer
- [x] Basic Log Viewer
- [x] Basic Texture2D Viewer
- [x] Runtime Shader Rebuild
- [x] Basic Resource Explorer
- [ ] Node Based Shader Editor
- [x] Sample FrameBuffers Viewer

-------------------------------------------------------

## **渲染管线(Pipeline)**

![20240601230356](https://github.com/tzkt623/TezcatEngine-Tiny/assets/6510903/77c7bbee-50df-4247-bee1-03b3d7c8e124)

- ObserverPipelinePass
  
  用于玩家视角相机的渲染通道,用于自动处理并渲染MeshRender自带的Material

- ReplacedPipePass
  
  用于用户自定义渲染通道,可以处理各种自定义需求的渲染,例如阴影,后处理等

- RenderCommand Order
  
|   OrderID    |   Bit   | Byte  |  Range   |
| :----------: | :-----: | :---: | :------: |
| Camera Order | 24 - 31 |   1   | -127,128 |
| Shader Queue | 16 - 23 |   1   |  0,256   |
|  Pass Order  | 0 - 15  |   2   | 0,65535  |

Sort Function ```a->getPipelineOrderID() < b->getPipelineOrderID()```

```cpp
//--------------------------------
//
//  Render Shadow
//
pass = ReplacedPipelinePass::create(mShadowObserver
    , ShaderManager::find("Unlit/ShadowMap"));
pass->setAutoCulling([](BaseMeshRenderer* renderer)
    {
        return new RenderCMD_DrawMeshWithOutMaterial(renderer->getVertex(), renderer->getTransform());
    });
pass->saveObject();
pass->addToPipeline();

//--------------------------------
//
//  Render Skybox
//
pass = ReplacedPipelinePass::create(CameraManager::getMainCamera()->getRenderObserver(), ShaderManager::find("Unlit/Skybox"));
pass->saveObject();
//not use RenderObject data in layer but custom function to rendering
sSkyBoxPass->setCustomCulling([=](ReplacedPipelinePass* pass)
{
    pass->addCommand<RenderCMD_DrawSkybox>(sSkyboxVertex
        , sCurrentCubeMap
        , sSkyboxLod
        , sCurrentCubeMap->getDataMemFormat().tiny == DataMemFormat::Float
        , sExposure);
});
pass->addToPipeline();

//--------------------------------
//
//  Render Screen Effect
//
auto shader = ShaderManager::find("Tutorial/Cube");
auto config_mvp = shader->getUniformValueConfig("MVP");
auto config_color = shader->getUniformValueConfig("myColor");

mPass = ReplacedPipelinePass::create(mObserver, shader);
mPass->setCustomCulling([=](ReplacedPipelinePass* pass)
{
    pass->addCommand<RenderCMD_Lambda>([=](PipelinePass* pass, Shader* shader)
        {
            auto projection = glm::perspective(glm::radians(75.0f), (floatEngine::getScreenWidth() / Engine::getScreenHeight(), 0.01f, 100.0f);
            auto view = glm::lookAt(float3(0.0f, 0.0f, 2.0f)
                , float3(0.0f, 0.0f, -1.0f)
                , float3(0.0f, 1.0f, 0.0f));
            auto model = glm::translate(float4x4(1.0f), float3(0.0f, 0.0f, -1.0f))
                * glm::rotate(float4x4(1.0f), 45.0f, float3(0.0f, 1.0f, 0.0f));
            auto mvp = projection * view * model;
            Graphics::getInstance()->setMat4(shader, config_mvp->valueID, mvp);
            Graphics::getInstance()->setFloat3(shader, config_color->valueID, float3(1.0f, 05f, 0.5f));
            Graphics::getInstance()->draw(mVertex);
        });
});
mPass->setFrameBuffer(FrameBufferManager::getMainFrameBufferBuildin());
mPass->addToPipeline();
```

## **编辑器(Editor)**

现在可以在菜单里面切换两个场景

### 着色器编辑器(Shader Editor)

双击资源管理器里的shader文件打开内置编辑器

### 场景总览(Scene Overview)

目前场景总览可以看到所有当前场景中的对象

### 对象总览(Object Overview)

目前对象总览可以看到对象上绑定的组件的信息,还可以与其互动

### 热编译着色器(Runtime Rebuild Shader)

可以在运行时重新编译着色器

### 资源管理器(Resource Browser)

双击图片文件可以打开查看

可以拖拽图片给环境光照贴图

### 帧缓存查看器(FrameBuffer Viewer)

查看当前的帧缓存信息

### 管线查看器(Pipeline Viewer)

查看当前的管线的渲染信息

## **内存管理(Memory Management)**

做了一套简单的基于引用计数的内存管理,还在调试中......

-----------------------------------------------

## **创建引擎对象**

TinyObject会自动进行内存管理

- 创建一个对象```YourType::create(...)```
- 保存一个对象```obj->saveObject()```
- 删除一个对象```obj->deleteObject()```

如果不调用```obj->saveObject()```方法,此对象会在当前帧结束时被删除

```cpp
//--------------------------------
//
// in .h file
//

//Abstract Object
class TINY_API PipelinePass : public TinyObject
{
    ...
    //Use This Macro To Define abstract TinyObject
    //TINY_ABSTRACT_OBJECT_H(CurrentType, ParentType)
    TINY_ABSTRACT_OBJECT_H(PipelinePass, TinyObject)
    //remember define protected Access to all Constructor
protected:
    PipelinePass(uint32 globalSubmitMask);    
public:
    virtual ~PipelinePass();
    ...
}

//Object
class TINY_API ReplacedPipelinePass : public PipelinePass
{
    ...
    //Use This Macro To Define TinyObject
    //TINY_OBJECT_H(CurrentType, ParentType)
    TINY_OBJECT_H(ReplacedPipelinePass, PipelinePass)
    //remember define protected or private Access to all Constructor
protected:
    ReplacedPipelinePass(BaseRenderObserver* renderObserver
        , Shader* shader
        , uint32 globalFunction = GLOBAL_NONE);
public:
    virtual ~ReplacedPipelinePass();
    ...
}


//--------------------------------
//
// in .cpp file
//

//Use this Macro to define TinyObject
//TINY_OBJECT_CPP(CurrentType, ParentType)
TINY_OBJECT_CPP(PipelinePass, TinyObject)
....
//Use this Macro to define TinyObject
//TINY_OBJECT_CPP(CurrentType, ParentType)
TINY_OBJECT_CPP(ReplacedPipelinePass, PipelinePass)
....

//--------------------------------
//
// create object
//
auto pass = ReplacedPipelinePass::create(render_observer, ShaderManager::find("Unlit/EnvMakeBRDFLut"));

````

## **创建游戏对象**

- 创建相机

```cpp
//gameobject will auto load into current scene
auto go = GameObject::create("World1_Camera");
//attach a Camera component
auto camera = go->addComponent<Camera>();
camera->setMain();
camera->setPerspective(60.0f, 0.1f, 2000.0f);
//set culling layer, other layers will not render
camera->setCullLayer(0);
//set clear option
camera->setClearOption(ClearOption::CO_Skybox | ClearOption::CO_Depth | ClearOption::CO_Color);

//attach a Transform component
go->addComponent<Transform>();
go->getTransform()->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
go->getTransform()->setParent(controller_go->getTransform());
```

- 创建一个区域光

```cpp
auto go = GameObject::create("DirectionLight");
go->addComponent<Transform>();
go->getTransform()->setPosition(glm::vec3(0.0f, 600.0f, 600.0f));
go->getTransform()->setScale(glm::vec3(100.0f));
go->getTransform()->setRotation(-60.0f, 0.0f, 0.0f);

auto dir_light = go->addComponent<DirectionalLight>();
dir_light->setDiffuse(glm::vec3(1.0f, 1.0f, 1.0f));
dir_light->setAmbient(glm::vec3(0.1f));
dir_light->setSpecular(glm::vec3(0.5f));

auto shadow_caster = go->addComponent<ShadowCaster>();
shadow_caster->setOrtho(0.1f, 2000.0f);
shadow_caster->setViewRect(0, 0, 1024, 1024);
shadow_caster->setCullLayer(0);
shadow_caster->setShadowMap(4096, 4096, "Shadow");
```

- 创建天空盒

  把资源管理器里面的一张方形天空盒直接拖到光照管理器中,记得把相机的ClearOption勾上Skybox

- 创建一个游戏物体

```cpp
auto wife = GameObject::create("Wife");
wife->addComponent<Transform>();
wife->getTransform()->setPosition(glm::vec3(-960.0f, 0.0f, 0.0f));
wife->getTransform()->setRotation(glm::vec3(0.0f, 90.0f, 0.0f));
wife->getTransform()->setScale(glm::vec3(1920.0f / 2, 1080.0f / 2, 1.0f));
wife->getTransform()->setParent(transform);

//add a MeshRenderer
auto mr2 = wife->addComponent<MeshRenderer>();
mr2->setMesh("Square");

auto wife_material2 = Material::create("Unlit/Texture");
mr2->setMaterial(wife_material2);

auto shader = wife_material2->getShader();

auto my_tex2d_color_index = shader->getUniformIndex("myTexColor2D");
auto tex = Resource::loadOnly<Texture2D>("Resource/Image/wife.jpg");
wife_material2->setUniform<UniformTex2D>(my_tex2d_color_index, tex);
```

- 加载模型

```cpp
auto model = ResourceManager::loadAndSave<Model>("Resource/Model/szb.fbx");
auto go_gundum_szb = model->generate();
go_gundum_szb->getTransform()->setPosition(157.74f, 106.560f, 150.780f);
go_gundum_szb->getTransform()->setRotation(-90.0f, 0.0f, 0.0f);
go_gundum_szb->getTransform()->setScale(10.0f, 10.0f, 10.0f);

model = ResourceManager::loadAndSave<Model>("Resource/Model/209790.fbx");
auto go_dasd = model->generate();
go_dasd->getTransform()->setPosition(0.0f, 0.0f, 90.0f);
go_dasd->getTransform()->setRotation(0.0f, -90.0f, 0.0f);
```

- 创建一个帧缓冲

```cpp
//create and save or get a framebuffer with manager
auto [flag2, frame_buffer] = FrameBufferManager::create("FB_Cube");
if (flag2 == FlagCreate::Succeeded)
{
    sCubeTextureMap = TextureCube::create("CB_CubeMap");
    sCubeTextureMap->setConfig(cube_size
        , TextureInternalFormat::RGB16F
        , TextureFormat::RGB
        , DataMemFormat::Float
        , TextureFilter::Linear_Mipmap_Linear
        , TextureFilter::Linear);
    sCubeTextureMap->setAttachPosition(TextureAttachPosition::ColorComponent);
    sCurrentCubeMap = sCubeTextureMap;

    auto render_buffer = Texture2D::create("Depth");
    render_buffer->setConfig(cube_size, cube_size
        , TextureInternalFormat::Depth
        , TextureFormat::Depth
        , DataMemFormat::Float);
    render_buffer->setAttachPosition(TextureAttachPosition::DepthComponent);
    frame_buffer->addAttachment(sCubeTextureMap);
    frame_buffer->addAttachment(render_buffer);
    frame_buffer->generate();
}

//create a framebuffer not saved in manager
mFrameBuffer = FrameBuffer::create(shaderName + std::to_string(mUID));
//remember call saveObject to save this object
mFrameBuffer->saveObject();
mFrameBuffer->addAttachment(mShadowTexture);
mFrameBuffer->generate();
```

- 创建一张贴图

```cpp
//create and save or get a texture named "MyTexture" with manager
auto [flag, texture] = TextureManager::create2D("MyTexture");
if (flag == FlagCreate::Succeeded)
{
    texture->setConfig(width, height
        , TextureInternalFormat::RGBA
        , TextureFormat::RGBA
        , DataMemFormat::UByte);
    texture->generate();
}

//---------------------------------------

//create a texture with image and not save in manager
auto image = Resource::loadOnly<Image>("Resource/Image/blocky_photo_studio_2k.hdr");
texture = Texture2D::create("MyImage");
//remember call saveObject to save this object
texture->saveObject();
texture->setImage(image);
texture->generate();
```

**具体使用方法请看Example.**

## **资源管理**

注意! .exe文件必须和资源文件夹处于同一目录下


1. 继承并实现`MyEngineIniter`类

    ```cpp
    class EditorEngineIniter : public EngineIniter
    {
    public:
        MyEngineIniter();

        void prepareEngine(Engine* engine) override;
        void prepareResource(Engine* engine) override;
        void prepareGame(Engine* engine) override;
        void initYourShaderParam() override;
    };
    ```

    `prepareEngine`,`prepareResource`,`prepareGame`三个方法会按此顺序依次执行

2. 设置自己的资源文件夹名称,程序名称,屏幕大小

    ```cpp
    void EditorEngineIniter::prepareEngine(Engine* engine)
    {
        EngineIniter::prepareEngine(engine);
        MyEvent::get()->init(MyEventID::Count);

        //engine->setEnableMultiThread();

        mGameName = u8"V0.2(没有黄金树之影玩我要死了)";
        mWindowWidth = 1920;
        mWindowHeight = 1080;
        mEnableVsync = true;
        this->setGLVersion(3, 3);
    }
    ```

3. 加载资源文件

    ```cpp
    void EditorEngineIniter::prepareResource(Engine* engine)
    {
        EngineIniter::prepareResource(engine);
    }
    ```

4. 准备场景

    ```cpp
    void EditorEngineIniter::prepareGame(Engine* engine)
    {
        EngineIniter::prepareGame(engine);
        ShaderManager::loadShaderFiles("Resource/Shaders/Tutorial");

        MyMainScene::create("MainScene")->prepare();
        SceneHelper::prepareTutorialScene();
    }
    ```

## **材质结构**

目前Shader构建器可以自动解析出所有Uniform变量.

Tiny的全局变量如下


|        ShaderName        |   Type   |        Useage         |
| :----------------------: | :------: | :-------------------: |
|       TINY_MatrixP       | float4x4 |           P           |
|       TINY_MatrixV       | float4x4 |           V           |
|       TINY_MatrixM       | float4x4 |           M           |
|      TINY_MatrixVP       | float4x4 |          VP           |
|      TINY_MatrixMVP      | float4x4 |          MVP          |
|       TINY_MatrixN       | float3x3 |  Model Normal Matrix  |
|    TINY_MatrixLightVP    | float4x4 |    Light VP Matrix    |
| TINY_CameraWorldPosition |  float3  | Camera World Position |
|    TINY_CameraNearFar    |  float2  |    Camera NearFar     |
|     TINY_Resolution      |  float2  |   Screen Resolution   |
|      TINY_TexSkybox      | texCube  |    Skybox Texture     |
|      TINY_TexDepth       |  tex2D   |     Depth Texture     |
|    TINY_TexIrradiance    | texCube  |  Irradiance Texture   |
|    TINY_TexPrefilter     | texCube  |   Prefilter Texture   |
|     TINY_TexBRDFLUT      |  tex2D   |    BRDFLUT Texture    |

|      ShaderName       |  Type  | Useage |
| :-------------------: | :----: | :----: |
| TINY_LitDir.direction | float3 |        |
|  TINY_LitDir.ambient  | float3 |        |
|  TINY_LitDir.diffuse  | float3 |        |
| TINY_LitDir.specular  | float3 |        |

|       ShaderName       |  Type  | Useage |
| :--------------------: | :----: | :----: |
| TINY_LitPoint.position | float3 |        |
| TINY_LitPoint.ambient  | float3 |        |
| TINY_LitPoint.diffuse  | float3 |        |
| TINY_LitPoint.specular | float3 |        |
|  TINY_LitPoint.config  | float3 |        |

**目前内建材质变量类型有**

|    TinyType     |  CommonType   |
| :-------------: | :-----------: |
|  UniformI[1-4]  |   int[1-4]    |
|  UniformF[1-4]  |  float[1-4]   |
| UniformMat[3-4] | glm::mat[3-4] |
|  UniformTex2D   |   Texture2D   |
| UniformTexCube  |  TextureCube  |

给一个object添加一个材质之后一定要记得添加材质对应的参数,贴图,数值等等

```cpp
auto plane_material = Material::create("Standard/Std1");
mr->setMaterial(plane_material);

auto shader = plane_material->getShader();
//use shader to find custom uniform`s index
auto index_diffuse = shader->getUniformIndex("myTexDiffuse2D");
auto index_specular = shader->getUniformIndex("myTexSpecular2D");
auto index_shininess = shader->getUniformIndex("myShininess");

auto tex_diff = Resource::loadOnly<Texture2D>("Resource/Image/stone_wall_diff.jpg");
auto tex_spec = Resource::loadOnly<Texture2D>("Resource/Image/stone_wall_ao.jpg");

//set value by using uniform index
plane_material->setUniform<UniformTex2D>(index_diffuse, tex_diff);
//or use your uniform`s name
//plane_material->setUniform<UniformTex2D>("myTexDiffuse2D", tex_diff);
plane_material->setUniform<UniformTex2D>(index_specular, tex_spec);
plane_material->setUniform<UniformF1>(index_shininess, 64.0f);
```

## **着色器**

现在着色器构建器采用组合头文件的方式来自动生成着色器文件

```cpp
void EngineIniter::prepareResource(Engine* engine)
{
    ShaderManager::loadShaderFiles("Resource/Shaders/Standard");
    ShaderManager::loadShaderFiles("Resource/Shaders/Unlit");
    ShaderManager::loadShaderFiles("Resource/Shaders/Utility");

    this->createSomeMode();
}
```

你可以在Include文件夹中建立一些通用头文件,避免在各个着色器文件中重复书写

头文件支持重复包含

着色器文件支持//和/**/两种注释

```glsl
file tiny_vs.tyin 
//base
uniform float4x4 TINY_MatrixP;
uniform float4x4 TINY_MatrixV;
uniform float4x4 TINY_MatrixM;
uniform float4x4 TINY_MatrixMV;
uniform float4x4 TINY_MatrixVP;
uniform float4x4 TINY_MatrixMVP;
uniform float3x3 TINY_MatrixN;

//light
uniform float4x4 TINY_MatrixLightVP;

file any shader you need
#TINY_VS_BEGIN
{
    #include "../Include/tiny_vs.tyin"
    ..........
}
```

### **管线位置**

```cpp
"Background"    Forward::Background
"Opaque"        Forward::Geometry
"Alpha"         Forward::AlphaTest
"OpaqueLast"    Forward::OpaqueLast
"Transparent"   Forward::Transparent
"Overlay"       Forward::Overlay
```

### **混合**

混合参数

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

启用混合

```cpp
bool Blend = true;
str BlendSrc = 1;
str BlendTar = 1-TarA;
```

关闭混合

```cpp
bool Blend = false;
```

### **表面剔除**

剔除参数

```cpp
"Off"       Disable
"Front"     FRONT
"Back"      BACK
"All"       FRONT_AND_BACK
```

启用剔除

```cpp
str CullFace = Back;
```

关闭剔除

```cpp
str CullFace = Off;
```

### **深度测试**

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

启用

```cpp
str DepthTest = Less;
```

关闭

```cpp
str DepthTest = Off;
```

### **深度写入**

只有在深度测试启用时才有用

启用

```cpp
bool ZWrite = true;
```

关闭

```cpp
bool ZWrite = false;
```

### **内建宏**

|        Macro         |              glsl               |      Parameter       |
| :------------------: | :-----------------------------: | :------------------: |
| TINY_LAYOUT_POSITION | layout (location = 0) in float3 | tiny_layout_Position |
|  TINY_LAYOUT_NORMAL  | layout (location = 1) in float3 |  tiny_layout_Normal  |
|    TINY_LAYOUT_UV    | layout (location = 2) in float2 |    tiny_layout_UV    |
|  TINY_LAYOUT_COLOR   | layout (location = 3) in float4 |  tiny_layout_Color   |

### ***参数属性***

格式```@[AttributeType(Arg1(), Arg2(),.....)]```

```glsl
//EditorView
struct TinyPBR_Test
{
    @[Editor(Name(Albedo), Color())]
    float3 albedo;
    @[Editor(Name(Metallic), Range(0, 1))]
    float metallic;
    @[Editor(Name(Roughness), Range(0, 1))]
    float roughness;
    @[Editor(Name(AO), Range(0, 1))]
    float ao;
};

//UBO Index
@[Binding(0)]
layout(std140) uniform CameraUBO
{
    float4x4 TINY_MatrixP;
    float4x4 TINY_MatrixV;
    float4x4 TINY_MatrixVP;
    float3 TINY_CameraWorldPosition;
    float2 TINY_CameraNearFar;
};
```

### **默认值**

除了[`int Version`]为必须值,其他值均为拥有默认值的可选参数

```glsl
#TINY_HEAD_BEGIN
{
    str Name = Standard/Std1;
}
#TINY_HEAD_END

#TINY_PASS_BEGIN
{
    #TINY_CFG_BEGIN
    {
        str Name = Standard/Std1;
        int Version = 330;
        int OrderID = 50;
        str Queue = Opaque;
        str DepthTest = Less;
        bool ZWrite = true;
        str CullFace = Back;
        str LightMode = Forward;
    }
    #TINY_CFG_END

    #TINY_VS_BEGIN
    {
        #include "../Include/tiny_vs.tyin"

        TINY_LAYOUT_POSITION;
        TINY_LAYOUT_NORMAL;
        TINY_LAYOUT_UV;
        TINY_LAYOUT_COLOR;

        out float4 myColor;
        out float2 myUV;
        out float3 myNormal;
        out float3 myWorldPosition;
        out float4 myLightPosition;

        void main()
        {
            float4 position =  float4(tiny_layout_Position, 1.0);
            gl_Position = TINY_MatrixP * TINY_MatrixV * TINY_MatrixM * position;

            myColor = tiny_layout_Color;
            myUV = tiny_layout_UV;
            myNormal = TINY_MatrixN * tiny_layout_Normal;
            myWorldPosition = float3(TINY_MatrixM * position);
            myLightPosition = TINY_MatrixLightVP * float4(myWorldPosition, 1.0f);
        }
    }
    #TINY_VS_END

    #TINY_FS_BEGIN
    {
        #include "../Include/tiny_fs.tyin"

        in float4 myColor;
        in float2 myUV;
        in float3 myNormal;
        in float3 myWorldPosition;
        in float4 myLightPosition;
        
        out float4 myFinalColor;

        uniform tex2D myTexDiffuse2D;
        uniform tex2D myTexSpecular2D;
        uniform float myShininess;

        float4 reflection(float3 I)
        {
            //float3 I = normalize(myWorldPosition - TINY_CameraWorldPosition);
            float3 R = reflect(I, normalize(myNormal));
            return float4(texture(TINY_TexSkybox, R).rgb, 1.0);
        }

        float4 refraction(float3 I)
        {
            float ratio = 1.00 / 1.52;
            //float3 I = normalize(myWorldPosition - TINY_CameraWorldPosition);
            float3 R = refract(I, normalize(myNormal), ratio);
            return float4(texture(TINY_TexSkybox, R).rgb, 1.0);
        }

        float3 calcDirectionLight(LightDirection lit, float3 viewDir, float3 normal)
        {
            float3 light_dir = normalize(-lit.direction);
            float3 half_dir = normalize(viewDir + light_dir); 

            float NdL = dot(normal, light_dir);
            float NdH = dot(normal, half_dir);

            float diff = max(NdL, 0.0);
            float spec = pow(max(NdH, 0.0), myShininess);

            float3 ambient = lit.ambient * texture(myTexDiffuse2D, myUV).rgb;
            float3 diffuse = lit.diffuse * diff * texture(myTexDiffuse2D, myUV).rgb;
            float3 specular = lit.specular * spec * texture(myTexSpecular2D, myUV).rrr;

            // shadow
            float shadow = calcShadow(myLightPosition, normal, light_dir, TINY_TexDepth);
            float3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular));

            return lighting;
        }

        void main()
        {
            float3 normal = normalize(myNormal);
            float3 view_dir = normalize(TINY_CameraWorldPosition - myWorldPosition);
            myFinalColor = float4(calcDirectionLight(TINY_LitDir, view_dir, normal), 1.0f);
        }
    }
    #TINY_FS_END
}
#TINY_PASS_END
```

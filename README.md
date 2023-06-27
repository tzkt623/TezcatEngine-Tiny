# TezcatEngine-Tiny

## **引擎二周目进行中**

![示例](https://github.com/tzkt623/TezcatEngine-Tiny/blob/main/logo1.jpg?raw=true)

Update

- [x] 修改了整个shader体系的结构,现在不再有特殊内建变量,只有全局通用变量(The shader system has been modified so that there are no longer special built-in variables, only global common variables)
- [x] 简化了shader头文件的包含,只需要包含一个通用的tiny头文件就行,如果不包含,可以自己写(Simplifies the inclusion of shader header files, only need to include a generic Tiny header file, if not, you can write your own)
- [x] 可以自己缓存uniform变量的index来快速更新数据(You can cache the index of uniform variables yourself to quickly update the data)
- [x] 基础模型加载(Basic Model load)
- [ ] 正在重构资源加载和管理方式(Refactoring resoure loader and managers)

## **依赖库版本(Libs Version)**

- C++20(v143)
- Windows sdk 10.0.22000.0
- IDE VS2022

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

## **编辑器(Editor)**

现在可以在菜单里面切换两个场景

Now you can switch scenes in the menu

### 着色器编辑器(Shader Editor)

双击资源管理器里的shader文件打开内置编辑器

Double-click the shader file in Explorer to open the built-in editor

### 场景总览(Scene Overview)

目前场景总览可以看到所有当前场景中的对象

Now Scene Overview can show all object in current scene, and you can select one of them

### 对象总览(Object Overview)

目前对象总览可以看到对象上绑定的组件的信息,还可以与其互动

Now Object Overview can show components bind in object when you selected, and you can change them values

### 热编译着色器(Runtime Rebuild Shader)

可以在运行时重新编译着色器

Now Editor can rebuild shader in runtime.

### 资源管理器(Resource Browse)

双击图片文件可以打开查看

Double Click image file to open viwer

可以拖拽图片给环境光照贴图

Try Drag image file to envlighting map widget

## **内存管理(Memory Management)**

做了一套简单的基于引用计数的内存管理,还在调试中......

A simple reference counting based memory management, just still debugging......

## **创建游戏对象(Create GameObjects)**

- 创建相机
  Create a Camera

```cpp
//gameobject will auto load into current scene
auto go = GameObject::create("World1_Camera");
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

  把资源管理器里面的一张方形天空盒直接拖到光照管理器中,记得把相机的ClearOption勾上Skybox

  Drag a png/jpg/hdr image in resource browser to lighting manager window And check camera`s clear option to render skybox

- 创建一个游戏物体
  Create a GameObject

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
auto tex = Resource::loadOnly<Texture2D>("Image/wife.jpg");
wife_material2->setUniform<UniformTex2D>(my_tex2d_color_index, tex);
```

- Load A Model

```cpp
auto model = Resource::load<Model>("Model/Cerberus_LP.fbx");
model->generate();
```

- 创建一个帧缓冲
  Create a FrameBuffer

```cpp
//create a framebuffer named "FB_Viewport" and save in manager
auto frame_buffer = FrameBufferMgr::getInstance()->create("FB_Viewport");

//create a texture2D named "RB_Viewport" and save in manager
Texture2D* tex2d = TextureMgr::getInstance()->create2D("RB_Viewport");
tex2d->setData(Engine::getScreenWidth(), Engine::getScreenHeight()
    , TextureInfo(
        //Buffer Type(FrameBuffer Component)
        TextureAttachPosition::ColorComponent
        //Internal Format
        , TextureChannel::RGBA
        //Format
        , TextureChannel::RGBA
        //Data Type
        , DataType::UByte));

//create a render2D named "DS_Viewport" and save in manager
TextureRender2D* render2d = TextureMgr::getInstance()->createRender2D("DS_Viewport");
render2d->setData(Engine::getScreenWidth(), Engine::getScreenHeight()
    , TextureInfo(TextureAttachPosition::DepthComponent
        , TextureChannel::Depth
        , TextureChannel::Depth
        , DataType::UByte));

//attach textures into framebuffer
frame_buffer->addAttachment(tex2d);
frame_buffer->addAttachment(render2d);
//generate framebuffer
//in multi-thread mode, this method just make a CMD and send it to render-thread
frame_buffer->generate();

//let camera render objects to this framebuffer
camera->setFrameBuffer(frame_buffer);

//also you can find this framebuffer like
fb = FrameBufferMgr::getInstance()->find("FB_World1");

//set nullptr will switch render to mainframe
camera->setFrameBuffer(nullptr);
```

- 创建一张贴图(Create a Texture)

```cpp
//create a texture named "Shadow" and save in manager
mShadwowTexutre = TextureMgr::getInstance()->create2D("Shadow");
mShadwowTexutre->setData(width, height
    , TextureInfo(TextureType::Texture2D
        , TextureAttachPosition::DepthComponent
        , TextureFilter::Nearest
        , TextureFilter::Nearest
        , TextureWrap::Clamp_To_Border
        , TextureWrap::Clamp_To_Border
        , TextureChannel::Depth
        , TextureChannel::Depth
        , DataType::Float32));
mShadwowTexutre->generate();
```

**具体使用方法请看Example.**

**Please check the Example project to get more infos.**

## **资源管理 Resouce Manage**

注意! .exe文件必须和资源文件夹处于同一目录下

Attention! The .exe file must be in the same directory as the resource folder

1. 继承并实现`MyEngineIniter`类

   Inherit and implement the `MyEngineIniter` class

    ```cpp
    class MyEngineIniter : public EngineIniter
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

    `prepareEngine`,`prepareResource`,`prepareGame`are invoked in this order

2. 设置自己的资源文件夹名称,程序名称,屏幕大小

   Set your **ResourceFolder Name, ProgramName, ScreenSize**

    ```cpp
    void MyEngineIniter::prepareEngine(Engine* engine)
    {
        EngineIniter::prepareEngine(engine);
        MyEvent::get()->init(MyEventID::Count);

        engine->setEnableMultiThread();

        mResourceFolderName = "Resource";
        mGameName = u8"YesIndeed,玩上老头环了!!!!!";
        mWindowWidth = 1920;
        mWindowHeight = 1080;
        mEnableVsync = true;
        this->setGLVersion(3, 3);
    }
    ```

3. 加载资源文件

   Load resource files

    ```cpp
    void MyEngineIniter::prepareResource(Engine* engine)
    {
        EngineIniter::prepareResource(engine);
        //设置图片文件夹名称自动加载所有图片文件
        //注意,不同文件夹下面的图片文件也不能重名
        //Set ImageFolder to auto load all images
        //Note that the image files under different folders also must not have the same name
        TextureMgr::getInstance()->loadResource("/Image");
    }
    ```

4. 准备场景

   Prepare Scene

    ```cpp
    void MyEngineIniter::prepareGame(Engine* engine)
    {
        EngineIniter::prepareGame(engine);
        ShaderMgr::getInstance()->loadShaderFiles(FileTool::getRootRelativeResDir() + "/Shaders/Tutorial");

        auto main_window = new MyMainWindow();
        main_window->open(Graphics::getInstance()->mGUI);
        main_window->init();

        SceneMgr::getInstance()->prepareScene(MyMainScene::create("MainScene"));
        SceneMgr::getInstance()->prepareScene(Tutorial01::create("Tutorial01"));
    }
    ```

## **材质结构 Material**

目前Shader构建器可以自动解析出所有Uniform变量.

ShaderBuilder can auto scan all GLSL Uniform value except array type.

Tiny的全局变量如下

Tiny Current Global Uniform Values

|        ShaderName        | CommonType  |         Useage          |
| :----------------------: | :---------: | :---------------------: |
|       TINY_MatrixP       |    mat4     |            P            |
|       TINY_MatrixV       |    mat4     |            V            |
|       TINY_MatrixM       |    mat4     |            M            |
|      TINY_MatrixVP       |    mat4     |           VP            |
|      TINY_MatrixMVP      |    mat4     |           MVP           |
|       TINY_MatrixN       |    mat3     |   Model Normal Matrix   |
|    TINY_MatrixLightVP    |    mat4     |       Light`s VP        |
| TINY_CameraWorldPosition |    vec3     | Camera`s World Position |
|    TINY_CameraNearFar    |    vec2     |    Camera`s NearFar     |
|     TINY_Resolution      |    vec2     |    Screen Resolution    |
|      TINY_TexSkybox      | TextureCube |     Skybox Texture      |
|      TINY_TexDepth       |  Texture2D  |      Depth Texture      |
|    TINY_TexIrradiance    | TextureCube |   Irradiance Texture    |
|    TINY_TexPrefilter     | TextureCube |    Prefilter Texture    |
|     TINY_TexBRDFLUT      |  Texture2D  |     BRDFLUT Texture     |

|      ShaderName       | CommonType | Useage |
| :-------------------: | :--------: | :----: |
| TINY_LitDir.direction |    vec3    |        |
|  TINY_LitDir.ambient  |    vec3    |        |
|  TINY_LitDir.diffuse  |    vec3    |        |
| TINY_LitDir.specular  |    vec3    |        |

|       ShaderName       | CommonType | Useage |
| :--------------------: | :--------: | :----: |
| TINY_LitPoint.position |    vec3    |        |
| TINY_LitPoint.ambient  |    vec3    |        |
| TINY_LitPoint.diffuse  |    vec3    |        |
| TINY_LitPoint.specular |    vec3    |        |
|  TINY_LitPoint.config  |    vec3    |        |

**目前内建材质变量类型有**(后续会慢慢添加)

Current Buildin Material Values

|    TinyType     |  CommonType   |
| :-------------: | :-----------: |
|  UniformI[1-4]  |   int[1-4]    |
|  UniformF[1-4]  |  float[1-4]   |
| UniformMat[3-4] | glm::mat[3-4] |
|  UniformTex2D   |   Texture2D   |
| UniformTexCube  |  TextureCube  |

给一个object添加一个材质之后一定要记得添加材质对应的参数,贴图,数值等等

notice! add uniform value to your material for the gameobject.

```cpp
auto plane_material = Material::create("Standard/Std1");
mr->setMaterial(plane_material);

auto shader = plane_material->getShader();
//use shader to find custom uniform`s index
auto index_diffuse = shader->getUniformIndex("myTexDiffuse2D");
auto index_specular = shader->getUniformIndex("myTexSpecular2D");
auto index_shininess = shader->getUniformIndex("myShininess");

auto tex_diff = Resource::loadOnly<Texture2D>("Image/stone_wall_diff.jpg");
auto tex_spec = Resource::loadOnly<Texture2D>("Image/stone_wall_ao.jpg");

//set value by using uniform index
plane_material->setUniform<UniformTex2D>(index_diffuse, tex_diff);
//or use your uniform`s name
//plane_material->setUniform<UniformTex2D>("myTexDiffuse2D", tex_diff);
plane_material->setUniform<UniformTex2D>(index_specular, tex_spec);
plane_material->setUniform<UniformF1>(index_shininess, 64.0f);
```

## **着色器 Shader**

现在着色器构建器采用组合头文件的方式来自动生成着色器文件

ShaderBuilder now combine header files to automatically generate a shader file

```cpp
void EngineIniter::prepareResource(Engine* engine)
{
    ShaderMgr::getInstance()->loadShaderFiles(FileTool::getRootRelativeResDir() + "/Shaders/Standard");
    ShaderMgr::getInstance()->loadShaderFiles(FileTool::getRootRelativeResDir() + "/Shaders/Unlit");
    ShaderMgr::getInstance()->loadShaderFiles(FileTool::getRootRelativeResDir() + "/Shaders/Utility");

    this->createSomeMode();
}
```

你可以在Include文件夹中建立一些通用头文件,避免在各个着色器文件中重复书写

You can create generic header files in the `Include` folder to avoid duplicating each shader file

头文件支持重复包含

Header files support repetitive inclusion

着色器文件支持//和/**/两种注释

The shader file supports both // and /**/

```glsl
file tiny_vs.tyin 
//base
uniform mat4 TINY_MatrixP;
uniform mat4 TINY_MatrixV;
uniform mat4 TINY_MatrixM;
uniform mat4 TINY_MatrixMV;
uniform mat4 TINY_MatrixVP;
uniform mat4 TINY_MatrixMVP;
uniform mat3 TINY_MatrixN;

//light
uniform mat4 TINY_MatrixLightVP;

file any shader you need
#TINY_VS_BEGIN
{
    #include "../Include/tiny_vs.tyin"
    ..........
}
```

### **管线位置 Pipeline Position**

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

### **默认值 DefaultValue**

除了[`int Version`]为必须值,其他值均为拥有默认值的可选参数

The[`int Version`] should be setted.The other params You can set as your wish.

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

    #TINY_VA_BEGIN
    {
        inout TINY_VS2FS
        {
            vec4 color;
            vec2 uv;
            vec3 normal;
            vec3 worldPosition;
            vec4 lightPosition;
        };
    }
    #TINY_VA_END

    #TINY_VS_BEGIN
    {
        #include "../Include/tiny_vs.tyin"

        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aNormal;
        layout (location = 2) in vec2 aUV;
        layout (location = 3) in vec4 aColor;

        out vec4 myColor;
        out vec2 myUV;
        out vec3 myNormal;
        out vec3 myWorldPosition;
        out vec4 myLightPosition;

        out VS2FS
        {
            vec4 color;
            vec2 uv;
            vec3 normal;
            vec3 worldPosition;
            vec4 lightPosition;
        } TINY_VS2FS;

        void main()
        {
            vec4 position =  vec4(aPos, 1.0);
            gl_Position = TINY_MatrixP * TINY_MatrixV * TINY_MatrixM * position;

            myColor = aColor;
            myUV = aUV;
            myNormal = TINY_MatrixN * aNormal;
            myWorldPosition = vec3(TINY_MatrixM * position);
            myLightPosition = TINY_MatrixLightVP * vec4(myWorldPosition, 1.0f);
        }
    }
    #TINY_VS_END

    #TINY_FS_BEGIN
    {
        #include "../Include/tiny_fs.tyin"

        in vec4 myColor;
        in vec2 myUV;
        in vec3 myNormal;
        in vec3 myWorldPosition;
        in vec4 myLightPosition;

        in VS2FS
        {
            vec4 color;
            vec2 uv;
            vec3 normal;
            vec3 worldPosition;
            vec4 lightPosition;
        } TINY_VS2FS;
        
        out vec4 myFinalColor;

        uniform sampler2D myTexDiffuse2D;
        uniform sampler2D myTexSpecular2D;
        uniform float myShininess;

        vec4 reflection(vec3 I)
        {
            //vec3 I = normalize(myWorldPosition - TINY_CameraWorldPosition);
            vec3 R = reflect(I, normalize(myNormal));
            return vec4(texture(TINY_TexSkybox, R).rgb, 1.0);
        }

        vec4 refraction(vec3 I)
        {
            float ratio = 1.00 / 1.52;
            //vec3 I = normalize(myWorldPosition - TINY_CameraWorldPosition);
            vec3 R = refract(I, normalize(myNormal), ratio);
            return vec4(texture(TINY_TexSkybox, R).rgb, 1.0);
        }

        vec3 calcDirectionLight(LightDirection lit, vec3 viewDir, vec3 normal)
        {
            vec3 light_dir = normalize(-lit.direction);
            vec3 half_dir = normalize(viewDir + light_dir); 

            float NdL = dot(normal, light_dir);
            float NdH = dot(normal, half_dir);

            float diff = max(NdL, 0.0);
            float spec = pow(max(NdH, 0.0), myShininess);

            vec3 ambient = lit.ambient * texture(myTexDiffuse2D, myUV).rgb;
            vec3 diffuse = lit.diffuse * diff * texture(myTexDiffuse2D, myUV).rgb;
            vec3 specular = lit.specular * spec * texture(myTexSpecular2D, myUV).rrr;

            // shadow
            float shadow = calcShadow(myLightPosition, normal, light_dir, TINY_TexDepth);
            vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular));

            return lighting;
        }

        void main()
        {
            vec3 normal = normalize(myNormal);
            vec3 view_dir = normalize(TINY_CameraWorldPosition - myWorldPosition);
            myFinalColor = vec4(calcDirectionLight(TINY_LitDir, view_dir, normal), 1.0f);
        }
    }
    #TINY_FS_END
}
#TINY_PASS_END
```

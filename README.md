# TezcatEngine-Tiny

## **引擎二周目进行中**

![示例](https://github.com/tzkt623/TezcatEngine-Tiny/blob/main/logo1.jpg?raw=true)

注意!本引擎使用C++20版本

Notice!Tiny Use The C++20 Ver.

## **编辑器(Editor)**

### 场景总览(Scene Overview)

目前场景总览可以看到所有当前场景中的对象

Now Scene Overview can show all object in current scene, and you can select one of them

### 对象总览(Object Overview)

目前对象总览可以看到对象上绑定的组件的信息,还可以与其互动

Now Object Overview can show components bind in object when you selected

### 热编译着色器(Runtime Rebuild Shader)

可以在运行时重新编译着色器

Now Editor an rebuild shader in runtime.

主菜单->Shader->Rebuild之下选你想重新编译的,或者选all全部编译

MainMenu->Shader->Rebuild, Choose your want or All

## **代码结构(Code)**

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

## **创建游戏对象(Create GameObjects)**

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

独立型变量 individual variable

|   TinyName   | CommonType  |       Useage        |
| :----------: | :---------: | :-----------------: |
|   MatrixP    |    mat4     |    ProjectionMat    |
|   MatrixV    |    mat4     |       ViewMat       |
|   MatrixM    |    mat4     |      ModelMat       |
|   MatrixN    |    mat3     |      NormalMat      |
|  MatrixSBV   |    mat4     |    SkyboxViewMat    |
|  MatrixLit   |    mat4     |   LightSpacePVMat   |
| ViewPosition |    vec3     |    ViewPosition     |
| ViewNearFar  |    vec2     | View`s Near And Far |
| VertexColor  |    vec4     |   Vertex`s Color    |
|   TexColor   |  Texture2D  |   Color`s Texture   |
|   TexCube    | TextureCube |   Cube`s Texture    |
|   TexDepth   |  Texture2D  |   Depth`s Texture   |

结构型变量 struct variable
|         TinyName          | CommonType |       Useage        |
| :-----------------------: | :--------: | :-----------------: |
|   StdMaterial::Diffuse    | Texture2D  |  Diffuse`s Texture  |
|    StdMaterial::Normal    | Texture2D  |  Normal`s Texture   |
|   StdMaterial::Specular   | Texture2D  | Specular`s Texture  |
|  StdMaterial::Shininess   |   float    | Specular  Shininess |
| LightDirection::Direction |    vec3    |                     |
|  LightDirection::Ambient  |    vec3    |                     |
|  LightDirection::Diffuse  |    vec3    |                     |
| LightDirection::Specular  |    vec3    |                     |
|   LightPoint::Position    |    vec3    |                     |
|    LightPoint::Ambient    |    vec3    |                     |
|    LightPoint::Diffuse    |    vec3    |                     |
|   LightPoint::Specular    |    vec3    |                     |
|    LightPoint::Config     |    vec3    |                     |

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
auto plane_material = new Material("Standard/Std1");
//texture is auto loaded by manager,so just put it`s name in function
plane_material->addUniform<UniformTex2D>(ShaderParam::StdMaterial::Diffuse, "stone_wall_diff");
plane_material->addUniform<UniformTex2D>(ShaderParam::StdMaterial::Specular, "stone_wall_ao");
plane_material->addUniform<UniformF1>(ShaderParam::StdMaterial::Shininess, 64.0f);
//also texturebuffer is cached by manager, just put it`s name in here
plane_material->addUniform<UniformTex2D>(ShaderParam::TexDepth, "Shadow");

auto mr = plane->addComponent<MeshRenderer>();
mr->setMaterial(plane_material);
```

## **着色器 Shader**

现在着色器构建器采用组合头文件的方式来自动生成着色器文件

ShaderBuilder now combine header files to automatically generate a shader file

```cpp
void ResourceLoader::prepareResource(Engine* engine)
{
    ShaderMgr::getInstance()->loadIncludeFiles(FileTool::getRootResDir() + "/Shaders/Include");

    ShaderMgr::getInstance()->create(FileTool::getRootResDir() + "/Shaders/Standard/std1.glsl");
    ShaderMgr::getInstance()->create(FileTool::getRootResDir() + "/Shaders/Standard/std2.glsl");
    ShaderMgr::getInstance()->create(FileTool::getRootResDir() + "/Shaders/Unlit/color.glsl");
    ShaderMgr::getInstance()->create(FileTool::getRootResDir() + "/Shaders/Unlit/color_depth.glsl");
    ShaderMgr::getInstance()->create(FileTool::getRootResDir() + "/Shaders/Unlit/texture.glsl");
    ShaderMgr::getInstance()->create(FileTool::getRootResDir() + "/Shaders/Unlit/texture_depth.glsl");
    ShaderMgr::getInstance()->create(FileTool::getRootResDir() + "/Shaders/Unlit/skybox.glsl");
    ShaderMgr::getInstance()->create(FileTool::getRootResDir() + "/Shaders/Unlit/transparent.glsl");
    ShaderMgr::getInstance()->create(FileTool::getRootResDir() + "/Shaders/Unlit/shadow_map.glsl");

    ShaderMgr::getInstance()->clearIncludeFiles();

    this->createSomeMode();
}
```

你可以在Include文件夹中建立一些通用头文件,避免在各个着色器文件中重复书写

You can create generic header files in the `Include` folder to avoid duplicating each shader file

头文件虽然支持重复包含,但请不要这样做

Header files support repetitive inclusion, but do not do so

着色器文件支持//和/**/两种注释

The shader file supports both // and /**/

```glsl
file tiny_vs_base.tysl 
uniform mat4 TINY_MatrixP;
uniform mat4 TINY_MatrixV;
uniform mat4 TINY_MatrixM;
uniform mat3 TINY_MatrixN;

file tiny_vs_shadow.tysl
uniform mat4 TINY_MatrixLit;

file any shader you need
#TINY_VS_BEGIN
{
    #include "tiny_vs_base"
    #include "tiny_vs_shadow"
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
        str Name = Std1;
        int Version = 330;
        int OrderID = 50;
        str Queue = Opaque;
        str DepthTest = Less;
        bool ZWrite = true;
        str CullFace = Back;
    }
    #TINY_CFG_END

    #TINY_VS_BEGIN
    {
        #include "tiny_vs_base"
        #include "tiny_vs_shadow"

        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aNormal;
        layout (location = 2) in vec4 aColor;
        layout (location = 3) in vec2 aUV;

        out vec4 myColor;
        out vec2 myUV;
        out vec3 myNormal;
        out vec3 myWorldPosition;
        out vec4 myLightPosition;

        void main()
        {
            vec4 position =  vec4(aPos, 1.0);
            gl_Position = TINY_MatrixP * TINY_MatrixV * TINY_MatrixM * position;

            myColor = aColor;
            myUV = aUV;
            myNormal = TINY_MatrixN * aNormal;
            myWorldPosition = vec3(TINY_MatrixM * position);
            myLightPosition = TINY_MatrixLit * vec4(myWorldPosition, 1.0f);
        }
    }
    #TINY_VS_END

    #TINY_FS_BEGIN
    {
        #include "tiny_fs_struct"
        #include "tiny_fs_camera"
        #include "tiny_fs_texture"

        in vec4 myColor;
        in vec2 myUV;
        in vec3 myNormal;
        in vec3 myWorldPosition;
        in vec4 myLightPosition;
        
        out vec4 myFinalColor;

        vec4 reflection(vec3 I)
        {
            //vec3 I = normalize(myWorldPosition - TINY_ViewPosition);
            vec3 R = reflect(I, normalize(myNormal));
            return vec4(texture(TINY_TexCube, R).rgb, 1.0);
        }

        vec4 refraction(vec3 I)
        {
            float ratio = 1.00 / 1.52;
            //vec3 I = normalize(myWorldPosition - TINY_ViewPosition);
            vec3 R = refract(I, normalize(myNormal), ratio);
            return vec4(texture(TINY_TexCube, R).rgb, 1.0);
        }

        float calcShadow(vec4 lightPosition, vec3 normal, vec3 lightDir)
        {
            // 执行透视除法
            vec3 projCoords = lightPosition.xyz / lightPosition.w;
            // 变换到[0,1]的范围
            projCoords = projCoords * 0.5 + 0.5;

            //超出投影的远平面范围
            if(projCoords.z > 1.0)
            {
                return 0.0;
            }

            // 取得最近点的深度(使用[0,1]范围下的lightPosition当坐标)
            float closestDepth = texture(TINY_TexDepth, projCoords.xy).r; 
            // 取得当前片段在光源视角下的深度
            float currentDepth = projCoords.z;

            float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

            // 检查当前片段是否在阴影中
            //float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

            // PCF
            float shadow = 0.0;
            vec2 texelSize = 1.0 / textureSize(TINY_TexDepth, 0);
            for(int x = -1; x <= 1; ++x)
            {
                for(int y = -1; y <= 1; ++y)
                {
                    float pcfDepth = texture(TINY_TexDepth, projCoords.xy + vec2(x, y) * texelSize).r; 
                    shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
                }    
            }
            shadow /= 9.0;

            return shadow;
        }

        vec3 calcDirectionLight(LightDirection lit, vec3 viewDir, vec3 normal)
        {
            vec3 light_dir = normalize(-lit.direction);

            float diff = max(dot(normal, light_dir), 0.0);

            //vec3 reflect_dir = reflect(-light_dir, normal);
            vec3 halfwayDir = normalize(light_dir + viewDir); 
            //float spec = pow(max(dot(viewDir, reflect_dir), 0.0), TINY_MatStd.shininess);
            float spec = pow(max(dot(normal, halfwayDir), 0.0), TINY_MatStd.shininess);

            vec3 ambient = lit.ambient * texture(TINY_MatStd.diffuse, myUV).rgb;
            vec3 diffuse = lit.diffuse * diff * texture(TINY_MatStd.diffuse, myUV).rgb;
            vec3 specular = lit.specular * spec * texture(TINY_MatStd.specular, myUV).rrr;

            // shadow
            float shadow = calcShadow(myLightPosition, normal, light_dir);
            vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular));

            return lighting;
            //return ambient + diffuse + specular;
            //return vec4(diffuse, 1.0f);
            //return vec4(texture(TINY_MatStd.diffuse, myUV).rgb, 1.0f);
        }

        void main()
        {
            vec3 normal = normalize(myNormal);
            vec3 view_dir = normalize(TINY_ViewPosition - myWorldPosition);

            myFinalColor = vec4(calcDirectionLight(TINY_LitDir, view_dir, normal), 1.0f);
            //myFinalColor = mix(calcDirectionLight(TINY_LitDir, viewDir, normal), reflection(-viewDir), 0.2f);
            //myFinalColor = reflection(-viewDir);
            //myFinalColor = refraction();
        }
    }
    #TINY_FS_END
}
#TINY_PASS_END
```

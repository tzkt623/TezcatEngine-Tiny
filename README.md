# TezcatEngine-Tiny

# 引擎二周目进行中

![示例](https://github.com/tzkt623/TezcatEngine-Tiny/blob/main/Resource/Image/logo.jpg?raw=true)
## 代码结构

因为我用unity比较多,就模仿了他的结构,现在的结构是这样
```c
auto texture_elden1 = new Texture();
texture_elden1->createTexture("../Resource/Image/eldenring1.jpg");

auto elden_ring1_material = new Material("Unlit/Texture");
elden_ring1_material->addTexture(TINY_TexColor, texture_elden1);

auto elden_ring1 = new GameObject();
auto mre1 = elden_ring1->addComponent<MeshRenderer>();
mre1->setMaterial(elden_ring1_material);
mre1->setMesh("Square");

elden_ring1->getTransform()->setPosition(glm::vec3(0.0f, 0.0f, -960.0f));
elden_ring1->getTransform()->setRotation(glm::vec3(0.0f, 0.0f, 0.0f));
elden_ring1->getTransform()->setScale(glm::vec3(1920.0f, 1080.0f, 1));
this->addGameObject(elden_ring1);

//-----------------------------

auto texture_elden2 = new Texture();
texture_elden2->createTexture("../Resource/Image/eldenring2.jpg");

auto elden_ring2_material = new Material("Unlit/Texture");
elden_ring2_material->addTexture(TINY_TexColor, texture_elden2);

auto elden_ring2 = new GameObject();
auto mre2 = elden_ring2->addComponent<MeshRenderer>();
mre2->setMaterial(elden_ring2_material);
mre2->setMesh("Square");

elden_ring2->getTransform()->setPosition(glm::vec3(0.0f, 0.0f, 960.0f));
elden_ring2->getTransform()->setRotation(glm::vec3(0.0f, -180.0f, 0.0f));
elden_ring2->getTransform()->setScale(glm::vec3(1920.0f, 1080.0f, 1));
this->addGameObject(elden_ring2);
```
## 材质结构
Shader系统的定义是这样
```c
#TINY_HEAD_BEGIN
{
    str Name = Unlit/Color;
}
#TINY_HEAD_END

#TINY_PASS_BEGIN
{
    #TINY_CFG_BEGIN
    {
        int Version = 330;
        int OrderID = 0;
        str Queue = Opaque;
    }
    #TINY_CFG_END

    #TINY_VS_BEGIN
    {
        layout (location = 0) in vec3 aPos;
        layout (location = 2) in vec4 aColor;

        uniform mat4 TINY_MatP;
        uniform mat4 TINY_MatV;
        uniform mat4 TINY_MatM;

        out vec4 myColor;


        void main()
        {
            vec4 position =  vec4(aPos, 1.0);
            gl_Position = TINY_MatP * TINY_MatV * TINY_MatM * position;
            
            myColor = aColor;
        }
    }
    #TINY_VS_END

    #TINY_FS_BEGIN
    {
        in vec4 myColor;

        out vec4 myFinalColor;

        void main()
        {
            myFinalColor = myColor;
        } 
    }
    #TINY_FS_END
}
#TINY_PASS_END
```
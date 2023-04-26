#include "MySecondScene.h"
#include "../MyInputer.h"
#include "../MyController.h"

TINY_RTTI_CPP(MySeconedScene)

MySeconedScene::MySeconedScene(const std::string& name)
	: Scene(name)
{
}

MySeconedScene::~MySeconedScene()
{

}

void MySeconedScene::onEnter()
{
	Scene::onEnter();
	InputSys::getInstance()->push(MyInputer::getInstance());


	if (true)
	{
		auto go = GameObject::create("World2_Camera");
		auto camera = go->addComponent<Camera>(true);
		camera->setViewRect(0, 0, Engine::getScreenWidth(), Engine::getScreenHeight());
		camera->setPerspective(60.0f, 0.1f, 2000.0f);
		camera->setPipeline(PipelineMgr::getInstance()->get("Forward"));
		camera->setCullLayer(0);
		camera->setDepth(1);

		MyInputer::getInstance()->setController(go->addComponent<FlyController>());

		go->addComponent<Transform>();
		go->getTransform()->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		//go->getTransform()->setParent(controller_go->getTransform());

// 		auto fb = FrameBufferMgr::getInstance()->create(
// 			"FB_World2",
// 			Engine::getScreenWidth(), Engine::getScreenHeight(),
// 			{
// 				TextureBufferInfo("RB_World2"
// 					, TextureBufferType::ColorComponent
// 					, TextureChannel::RGBA
// 					, TextureChannel::RGBA
// 					, DataType::UByte
// 					, true),
// 				TextureBufferInfo("DS_World2"
// 					, TextureBufferType::DepthStencilComponent
// 					, TextureChannel::Depth24_Stencil8)
// 			});

		auto frame = FrameBufferMgr::getInstance()->create(
					"FB_Viewport",
					Engine::getScreenWidth(), Engine::getScreenHeight(),
					{
						TextureBufferInfo("RB_Viewport"
							, TextureBufferType::ColorComponent
							, TextureChannel::RGBA
							, TextureChannel::RGBA
							, DataType::UByte
							, true),
						TextureBufferInfo("DS_Viewport"
							, TextureBufferType::DepthComponent
							, TextureChannel::Depth
							, TextureChannel::Depth
							, DataType::UByte
							, true)
					});

		camera->setFrameBuffer(frame);

		go = GameObject::create("Skybox");
		go->addComponent<Transform>();
		go->setLayerMaskIndex(0);

		auto skybox = go->addComponent<Skybox>();
		auto material = Material::create("Unlit/Skybox");
		material->addUniform<UniformTexCube>(ShaderParam::TexCube, "skybox_1");
		skybox->setMaterial(material);
	}

	//----------------------------------------
	//
	//	Cubes 1
	//
	if (true)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(-1000, 1000);

		for (int i = 0; i < 1000; i++)
		{
			auto go = GameObject::create();
			go->setLayerMaskIndex(0);
			go->addComponent<Transform>();
			go->getTransform()->setPosition(glm::vec3(dis(gen), dis(gen), dis(gen)));
			go->getTransform()->setScale(glm::vec3(10.0f));

			auto mr = go->addComponent<MeshRenderer>();
			auto material = Material::create("Standard/Std1");
			material->addUniform<UniformTex2D>(ShaderParam::StdMaterial::Diffuse, "metal_plate_diff");
			material->addUniform<UniformTex2D>(ShaderParam::StdMaterial::Specular, "metal_plate_spec");
			material->addUniform<UniformF1>(ShaderParam::StdMaterial::Shininess, 16.0f);
			//material->addUniform<UniformTexCube>(ShaderParam::TexCube, "skybox_2");
			mr->setMaterial(material);
			mr->setMesh("Sphere");
		}
	}
}

void MySeconedScene::onExit()
{
	Scene::onExit();
}

void MySeconedScene::onPause()
{

}

void MySeconedScene::onResume()
{

}


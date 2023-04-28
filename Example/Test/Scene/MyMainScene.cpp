#include "MyMainScene.h"
#include "../MyInputer.h"
#include "../MyController.h"


TINY_RTTI_CPP(MyMainScene)

MyMainScene::MyMainScene(const std::string& name)
	: Scene(name)
	, mController(nullptr)
{

}


void MyMainScene::onEnter()
{
	Scene::onEnter();
	InputSys::getInstance()->push(MyInputer::getInstance());

	float gateWidth = 1920.0f / 4;
	float gateHigh = 1080.0f / 4;

	mController = GameObject::create("Controller");
	mController->addComponent<Transform>();
	mController->getTransform()->setPosition(glm::vec3(0.0f, 0.0f, 10.0f));
	mController->addComponent<FlyController>();
	MyInputer::getInstance()->setController(mController->addComponent<FlyController>());

	this->createEnvMap();

	//地板
	if (true)
	{
		this->createInfinitePlane();
	}

	if (true)
	{
		auto go = GameObject::create("World1_Camera");
		auto camera = go->addComponent<Camera>(true);
		camera->setViewRect(0, 0, Engine::getScreenWidth(), Engine::getScreenHeight());
		camera->setPerspective(60.0f, 0.1f, 2000.0f);
		camera->setPipeline(PipelineMgr::getInstance()->get("Forward"));
		camera->setCullLayer(0);

		go->addComponent<Transform>();
		go->getTransform()->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		go->getTransform()->setRotation(glm::vec3(0.0f, 0.0f, 0.0f));
		go->getTransform()->setParent(mController->getTransform());
		//MyInputer::getInstance()->setController(go->addComponent<FlyController>());

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
		go->setLayerMaskIndex(0);
		go->addComponent<Transform>();

		auto skybox = go->addComponent<Skybox>();
		auto material = Material::create("Unlit/Skybox");
		material->addUniform<UniformTexCube>(ShaderParam::TexCube, "skybox_2");
		skybox->setMaterial(material);
	}

	if (false)
	{
		this->createGates(gateWidth, gateHigh);
	}

	//方向光
	//方向光的位置理论上不应该影响阴影投射
	if (true)
	{
		this->createDirectionLight();
	}

	//灯光深度图
	if (true)
	{
		auto lit_depth = GameObject::create("TextureDepth");
		lit_depth->addComponent<Transform>();
		lit_depth->getTransform()->setPosition(glm::vec3(960.0f, 0.0f, 0.0f));
		lit_depth->getTransform()->setRotation(glm::vec3(0.0f, -90.0f, 0.0f));
		lit_depth->getTransform()->setScale(glm::vec3(1024.0f, 1024.0f, 1.0f));
		//depth->getTransform()->setScale(glm::vec3(1920.0f, 1080.0f, 1.0f));

		auto depth_material = Material::create("Unlit/TextureDepth");
		depth_material->addUniform<UniformTex2D>(ShaderParam::TexColor, "Shadow");

		auto mr = lit_depth->addComponent<MeshRenderer>();
		mr->setMaterial(depth_material);
		mr->setMesh("Square");
	}

	///图片
	if (true)
	{
		this->createPaintings();
	}

	//----------------------------------------
	//
	//	Plane
	//
	if (true)
	{
		this->createPlane();
	}

	//----------------------------------------
	//
	//	Transparent
	//
	if (true)
	{
		this->createTransparentObject();
	}

	//PBR
	if (true)
	{
		this->createPBR();
	}

	//----------------------------------------
	//
	//	Cubes 0
	//
	if (false)
	{
		this->createCubes0();
	}

	if (true)
	{
		auto go = GameObject::create("HDRBox");
		//go->setLayerMaskIndex(0);
		go->addComponent<Transform>();
		go->getTransform()->setScale(glm::vec3(10.0f));

		auto mr = go->addComponent<MeshRenderer>();
		auto material = Material::create("Unlit/SkyboxHDR");
		material->addUniform<UniformTex2D>(ShaderParam::TexEnv, "blue_photo_studio_2k");
		mr->setMaterial(material);
		mr->setMesh("Cube");
	}
}

void MyMainScene::createGates(float gateWidth, float gateHigh)
{
	auto world1 = GameObject::create("World1_Gate");
	world1->setLayerMaskIndex(1);

	auto tran1 = world1->addComponent<Transform>();
	tran1->setPosition(glm::vec3(200.0f, 0.0f, 0.0f));
	tran1->setRotation(glm::vec3(0.0f, -90.0f, 0.0f));
	tran1->setScale(glm::vec3(gateWidth, gateHigh, 1.0f));

	auto mr1 = world1->addComponent<MeshRenderer>();
	auto world1_material = Material::create("Unlit/Texture");
	world1_material->addUniform<UniformTex2D>(ShaderParam::TexColor, "RB_World1");
	mr1->setMaterial(world1_material);
	mr1->setMesh("Square");


	auto world2 = GameObject::create("World2_Gate");
	auto tran2 = world2->addComponent<Transform>();
	tran2->setPosition(glm::vec3(-300.0f, 0.0f, 0.0f));
	tran2->setRotation(glm::vec3(0.0f, 90.0f, 0.0f));
	tran2->setScale(glm::vec3(gateWidth, gateHigh, 1.0f));

	auto mr2 = world2->addComponent<MeshRenderer>();
	auto world2_material = Material::create("Unlit/Texture");
	world2_material->addUniform<UniformTex2D>(ShaderParam::TexColor, "RB_World2");
	mr2->setMaterial(world2_material);
	mr2->setMesh("Square");
}

void MyMainScene::createPaintings()
{
	auto go = GameObject::create("Paintings");
	auto transform = go->addComponent<Transform>();
	transform->setPosition(glm::vec3(20.0f, 0.0f, -20.0f));

	//-------------------------------------------------
	auto wife = GameObject::create("Wife");
	wife->addComponent<Transform>();
	wife->getTransform()->setPosition(glm::vec3(-960.0f, 0.0f, 0.0f));
	wife->getTransform()->setRotation(glm::vec3(0.0f, 90.0f, 0.0f));
	wife->getTransform()->setScale(glm::vec3(1920.0f / 2, 1080.0f / 2, 1.0f));
	wife->getTransform()->setParent(transform);

	auto mr2 = wife->addComponent<MeshRenderer>();
	auto wife_material2 = Material::create("Unlit/Texture");
	wife_material2->addUniform<UniformTex2D>(ShaderParam::TexColor, "wife");
	mr2->setMaterial(wife_material2);
	mr2->setMesh("Square");


	//--------------------------------------
	auto elden_ring1 = GameObject::create("ED1");
	elden_ring1->addComponent<Transform>();
	elden_ring1->getTransform()->setPosition(glm::vec3(0.0f, 0.0f, -960.0f));
	elden_ring1->getTransform()->setRotation(glm::vec3(0.0f, 0.0f, 0.0f));
	elden_ring1->getTransform()->setScale(glm::vec3(1920.0f / 2, 1080.0f / 2, 1.0f));
	elden_ring1->getTransform()->setParent(transform);

	auto mre1 = elden_ring1->addComponent<MeshRenderer>();
	auto elden_ring1_material = Material::create("Unlit/Texture");
	elden_ring1_material->addUniform<UniformTex2D>(ShaderParam::TexColor, "eldenring1");
	mre1->setMaterial(elden_ring1_material);
	mre1->setMesh("Square");


	auto elden_ring2 = GameObject::create("ED2");
	elden_ring2->addComponent<Transform>();
	elden_ring2->getTransform()->setPosition(glm::vec3(0.0f, 0.0f, 960.0f));
	elden_ring2->getTransform()->setRotation(glm::vec3(0.0f, -180.0f, 0.0f));
	elden_ring2->getTransform()->setScale(glm::vec3(1920.0f / 2, 1080.0f / 2, 1.0f));
	elden_ring2->getTransform()->setParent(transform);

	auto mre2 = elden_ring2->addComponent<MeshRenderer>();
	auto elden_ring2_material = Material::create("Unlit/Texture");
	elden_ring2_material->addUniform<UniformTex2D>(ShaderParam::TexColor, "eldenring2");
	mre2->setMaterial(elden_ring2_material);
	mre2->setMesh("Square");
}

void MyMainScene::createPlane()
{
	auto plane = GameObject::create("Plane");
	plane->addComponent<Transform>();
	plane->getTransform()->setPosition(glm::vec3(0.0f, -50.0f, 0.0f));
	plane->getTransform()->setRotation(glm::vec3(0.0f, 0.0f, 0.0f));
	plane->getTransform()->setScale(glm::vec3(1.0f, 1.0f, 1.0f));

	auto mr = plane->addComponent<MeshRenderer>();
	auto plane_material = Material::create("Standard/Std1");
	plane_material->addUniform<UniformTex2D>(ShaderParam::StdMaterial::Diffuse, "stone_wall_diff");
	plane_material->addUniform<UniformTex2D>(ShaderParam::StdMaterial::Specular, "stone_wall_ao");
	plane_material->addUniform<UniformF1>(ShaderParam::StdMaterial::Shininess, 64.0f);
	//plane_material->addUniform<UniformTex2D>(ShaderParam::TexDepth, "Shadow");
	mr->setMaterial(plane_material);
	mr->setMesh("Plane");
}

void MyMainScene::createCubes0()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(-520, 520);
	std::uniform_int_distribution<> dis_ro(0, 359);

	for (int i = 0; i < 1000; i++)
	{
		auto go = GameObject::create();
		go->addComponent<Transform>();
		go->getTransform()->setPosition(glm::vec3(dis(gen), dis(gen), dis(gen)));
		go->getTransform()->setScale(glm::vec3(10.0f));
		go->getTransform()->setRotation(glm::vec3(dis_ro(gen), dis_ro(gen), dis_ro(gen)));
		//go->getTransform()->setParent(mController->getTransform());

		auto mr = go->addComponent<MeshRenderer>();
		auto material = Material::create("Standard/Std1");
		material->addUniform<UniformTex2D>(ShaderParam::StdMaterial::Diffuse, "metal_plate_diff");
		material->addUniform<UniformTex2D>(ShaderParam::StdMaterial::Specular, "metal_plate_spec");
		material->addUniform<UniformF1>(ShaderParam::StdMaterial::Shininess, 64.0f);
		material->addUniform<UniformTexCube>(ShaderParam::TexCube, "skybox_2");
		//material->addUniform<UniformTex2D>(ShaderParam::TexDepth, "Shadow");
		mr->setMaterial(material);
		mr->setMesh("Cube");
		//mr->setMesh("Sphere");
	}
}

void MyMainScene::createTransparentObject()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(-300, 300);

	auto go = GameObject::create("Transparent");
	auto transform = go->addComponent<Transform>();
	transform->setPosition(glm::vec3(0.0f, 0.0f, -20.0f));

	for (int i = 0; i < 20; i++)
	{
		auto go = GameObject::create(StringTool::stringFormat("T%d", i));
		go->addComponent<Transform>();
		go->getTransform()->setPosition(glm::vec3(dis(gen), dis(gen), dis(gen)));
		go->getTransform()->setScale(glm::vec3(30.0f, 30.0f, 1.0f));
		go->getTransform()->setParent(transform);

		auto mr = go->addComponent<MeshRenderer>();
		auto material = Material::create("Unlit/Transparent");
		material->addUniform<UniformTex2D>(ShaderParam::TexColor, "transparent_window");
		mr->setMaterial(material);
		mr->setMesh("Square");
	}
}

void MyMainScene::createInfinitePlane()
{
	Log::info("createInfinitePlane");
	auto go = GameObject::create("InfinitePlane");
	go->addComponent<Transform>()->setPosition(0.0f, 0.0f, 0.0f);
	auto mr = go->addComponent<MeshRenderer>();
	mr->setCastShadow(false);
	mr->setMesh("GridSquare");
	auto material = Material::create("Utility/InfiniteGrid");
	mr->setMaterial(material);
}

void MyMainScene::createDirectionLight()
{
	Log::info("createDirectionLight");
	auto direction_light_go = GameObject::create("DirectionLight");
	direction_light_go->addComponent<Transform>();
	direction_light_go->getTransform()->setPosition(glm::vec3(0.0f, 600.0f, 600.0f));
	direction_light_go->getTransform()->setScale(glm::vec3(100.0f));
	direction_light_go->getTransform()->setRotation(-60.0f, 0.0f, 0.0f);

	auto mr = direction_light_go->addComponent<MeshRenderer>();
	auto light_material = Material::create("Unlit/Color");
	mr->setCastShadow(false);
	mr->setMaterial(light_material);
	mr->setMesh("Sphere");

	auto dir_light = direction_light_go->addComponent<DirectionalLight>();
	dir_light->setOrtho(0.1f, 2000.0f);
	dir_light->setViewRect(0, 0, 1024, 1024);

	dir_light->setDiffuse(glm::vec3(1.0f, 1.0f, 1.0f));
	dir_light->setAmbient(glm::vec3(0.1f));
	dir_light->setSpecular(glm::vec3(0.5f));

	//float rotation = 0;
	dir_light->startLogic([=]()
	{
		direction_light_go->getTransform()->rotate(glm::vec3(0.0f, 10.0f * Engine::getDeltaTime(), 0.0f));
	});
}

void MyMainScene::createPBR()
{
	Log::info("createPBR");
	auto go = GameObject::create("PBR");
	auto transform = go->addComponent<Transform>();
	transform->setPosition(glm::vec3(0.0f, 0.0f, -20.0f));

	for (int y = 0; y <= 5; y++)
	{
		for (int x = 0; x <= 5; x++)
		{
			auto go = GameObject::create(StringTool::stringFormat("PBRBall_%d_%d", x, y));
			go->addComponent<Transform>();
			go->getTransform()->setPosition(glm::vec3(x * 30.0f, y * 30.0f, -60.0f));
			go->getTransform()->setScale(glm::vec3(10.0f));
			go->getTransform()->setRotation(glm::vec3(90.0f, 0.0f, 0.0f));
			go->getTransform()->setParent(transform);

			auto mr = go->addComponent<MeshRenderer>();
			auto material = Material::create("Standard/PBRTest1");
			material->addUniform<UniformF3>(ShaderParam::MatPBR_Test::Albedo, glm::vec3(1.0f, 0.0f, 0.0f));
			material->addUniform<UniformF1>(ShaderParam::MatPBR_Test::Metallic, x * 0.2f + 0.001f);
			material->addUniform<UniformF1>(ShaderParam::MatPBR_Test::Roughness, y * 0.2f + 0.001f);
			material->addUniform<UniformF1>(ShaderParam::MatPBR_Test::AO, 0.5f);
			mr->setMaterial(material);
			mr->setMesh("Sphere");
		}
	}
}

void MyMainScene::createEnvMap()
{
	// 	FrameBufferMgr::getInstance()->create("FB_Env",
	// 		512, 512,
	// 		{
	// 			TextureBufferInfo("TB_Env", tex)
	// 		});
}

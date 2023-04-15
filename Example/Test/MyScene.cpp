#include "MyScene.h"
#include "MyInputer.h"
#include "MyGUI.h"
#include "MyController.h"

MyScene::MyScene(const std::string& name)
	: Scene(name)
{

}

void MyScene::initGUI()
{
	auto info = new MyInfoWindow();
	info->open();

	auto object = new MyObjectWindow();
	object->open();

	auto camera = new MyMainCameraWindow();
	camera->open();
}



void MyScene::onEnter()
{
	Scene::onEnter();

	float gateWidth = 1920.0f / 4;
	float gateHigh = 1080.0f / 4;

	auto controller_go = new GameObject("Controller");
	controller_go->addComponent<Transform>();
	controller_go->getTransform()->setPosition(glm::vec3(0.0f, 0.0f, 10.0f));
	controller_go->addComponent<MyController>();

	if (true)
	{
		auto go = new GameObject("World1_Camera");
		auto camera = go->addComponent<Camera>(true);
		camera->setViewRect(0, 0, Engine::getScreenWidth(), Engine::getScreenHeight());
		camera->setPerspective(60.0f, 0.1f, 2000.0f);
		camera->setPipeline(PipelineMgr::getInstance()->get("Forward"));
		camera->setCullLayer(0);


		go->addComponent<Transform>();
		go->getTransform()->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		go->getTransform()->setParent(controller_go->getTransform());

		FrameBufferMgr::getInstance()->create(
			"FB_World1",
			Engine::getScreenWidth(), Engine::getScreenHeight(),
			{
				TextureBufferInfo("RB_World1"
					, TextureBufferType::ColorComponent
					, TextureChannel::RGBA
					, TextureChannel::RGBA
					, DataType::UByte
					, true),
				TextureBufferInfo("DS_World1"
					, TextureBufferType::DepthStencilComponent
					, TextureChannel::Depth24_Stencil8)
			});

		go = new GameObject("Skybox1");
		go->setLayerMaskIndex(0);
		go->addComponent<Transform>();

		auto skybox = go->addComponent<Skybox>();
		auto material = new Material("Unlit/Skybox");
		material->addUniform<UniformTexCube>(ShaderParam::TexCube, "skybox_2");
		skybox->setMaterial(material);
	}

	if (false)
	{
		auto go = new GameObject("World2_Camera");
		auto camera = go->addComponent<Camera>(false);
		camera->setViewRect(0, 0, Engine::getScreenWidth(), Engine::getScreenHeight());
		camera->setPerspective(60.0f, 0.1f, 2000.0f);
		camera->setPipeline(PipelineMgr::getInstance()->get("Forward"));
		camera->setCullLayer(1);
		camera->setDepth(1);

		go->addComponent<Transform>();
		go->getTransform()->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		go->getTransform()->setParent(controller_go->getTransform());

		auto fb = FrameBufferMgr::getInstance()->create(
			"FB_World2",
			Engine::getScreenWidth(), Engine::getScreenHeight(),
			{
				TextureBufferInfo("RB_World2"
					, TextureBufferType::ColorComponent
					, TextureChannel::RGBA
					, TextureChannel::RGBA
					, DataType::UByte
					, true),
				TextureBufferInfo("DS_World2"
					, TextureBufferType::DepthStencilComponent
					, TextureChannel::Depth24_Stencil8)
			});

		camera->setFrameBuffer(fb);


		go = new GameObject("Skybox2");
		go->addComponent<Transform>();
		go->setLayerMaskIndex(1);

		auto skybox = go->addComponent<Skybox>();
		auto material = new Material("Unlit/Skybox");
		material->addUniform<UniformTexCube>(ShaderParam::TexCube, "skybox_1");
		skybox->setMaterial(material);
	}

	if (false)
	{
		auto world1 = new GameObject("World1_Gate");
		world1->setLayerMaskIndex(1);

		auto tran1 = world1->addComponent<Transform>();
		tran1->setPosition(glm::vec3(200.0f, 0.0f, 0.0f));
		tran1->setRotation(glm::vec3(0.0f, -90.0f, 0.0f));
		tran1->setScale(glm::vec3(gateWidth, gateHigh, 1.0f));

		auto mr1 = world1->addComponent<MeshRenderer>();
		auto world1_material = new Material("Unlit/Texture");
		world1_material->addUniform<UniformTex2D>(ShaderParam::TexColor, "RB_World1");
		mr1->setMaterial(world1_material);
		mr1->setMesh("Square");


		auto world2 = new GameObject("World2_Gate");
		auto tran2 = world2->addComponent<Transform>();
		tran2->setPosition(glm::vec3(-300.0f, 0.0f, 0.0f));
		tran2->setRotation(glm::vec3(0.0f, 90.0f, 0.0f));
		tran2->setScale(glm::vec3(gateWidth, gateHigh, 1.0f));

		auto mr2 = world2->addComponent<MeshRenderer>();
		auto world2_material = new Material("Unlit/Texture");
		world2_material->addUniform<UniformTex2D>(ShaderParam::TexColor, "RB_World2");
		mr2->setMaterial(world2_material);
		mr2->setMesh("Square");
	}

	//方向光
	//方向光的位置理论上不应该影响阴影投射
	if (true)
	{
		auto direction_light_go = new GameObject();
		direction_light_go->addComponent<Transform>();
		direction_light_go->getTransform()->setPosition(glm::vec3(0.0f, 600.0f, 600.0f));
		direction_light_go->getTransform()->setScale(glm::vec3(100.0f));
		direction_light_go->getTransform()->setRotation(-60.0f, 0.0f, 0.0f);

		auto mr = direction_light_go->addComponent<MeshRenderer>();
		auto light_material = new Material("Unlit/Color");
		mr->setMaterial(light_material);
		mr->setMesh("Sphere");

		auto dir_light = direction_light_go->addComponent<DirectionalLight>();
		dir_light->setOrtho(0.1f, 2000.0f);
		dir_light->setViewRect(0, 0, 1024, 1024);


		dir_light->setDiffuse(glm::vec3(1.0f, 1.0f, 1.0f));
		dir_light->setAmbient(glm::vec3(0.5f));
		dir_light->setSpecular(glm::vec3(0.5f));
	}

	//灯光深度图
	if (true)
	{
		auto lit_depth = new GameObject();
		lit_depth->addComponent<Transform>();
		lit_depth->getTransform()->setPosition(glm::vec3(960.0f, 0.0f, 0.0f));
		lit_depth->getTransform()->setRotation(glm::vec3(0.0f, -90.0f, 0.0f));
		lit_depth->getTransform()->setScale(glm::vec3(1024.0f, 1024.0f, 1.0f));
		//depth->getTransform()->setScale(glm::vec3(1920.0f, 1080.0f, 1.0f));

		auto depth_material = new Material("Unlit/TextureDepth");
		depth_material->addUniform<UniformTex2D>(ShaderParam::TexColor, "Shadow");

		auto mr = lit_depth->addComponent<MeshRenderer>();
		mr->setMaterial(depth_material);
		mr->setMesh("Square");
	}

	if (true)
	{
		//-------------------------------------------------
		auto wife2 = new GameObject();
		wife2->addComponent<Transform>();
		wife2->getTransform()->setPosition(glm::vec3(-960.0f, 0.0f, 0.0f));
		wife2->getTransform()->setRotation(glm::vec3(0.0f, 90.0f, 0.0f));
		wife2->getTransform()->setScale(glm::vec3(1920.0f / 2, 1080.0f / 2, 1.0f));

		auto mr2 = wife2->addComponent<MeshRenderer>();
		auto wife_material2 = new Material("Unlit/Texture");
		wife_material2->addUniform<UniformTex2D>(ShaderParam::TexColor, "wife");
		mr2->setMaterial(wife_material2);
		mr2->setMesh("Square");


		//--------------------------------------
		auto elden_ring1 = new GameObject();
		elden_ring1->addComponent<Transform>();
		elden_ring1->getTransform()->setPosition(glm::vec3(0.0f, 0.0f, -960.0f));
		elden_ring1->getTransform()->setRotation(glm::vec3(0.0f, 0.0f, 0.0f));
		elden_ring1->getTransform()->setScale(glm::vec3(1920.0f / 2, 1080.0f / 2, 1.0f));

		auto mre1 = elden_ring1->addComponent<MeshRenderer>();
		auto elden_ring1_material = new Material("Unlit/Texture");
		elden_ring1_material->addUniform<UniformTex2D>(ShaderParam::TexColor, "eldenring1");
		mre1->setMaterial(elden_ring1_material);
		mre1->setMesh("Square");


		auto elden_ring2 = new GameObject();
		elden_ring2->addComponent<Transform>();
		elden_ring2->getTransform()->setPosition(glm::vec3(0.0f, 0.0f, 960.0f));
		elden_ring2->getTransform()->setRotation(glm::vec3(0.0f, -180.0f, 0.0f));
		elden_ring2->getTransform()->setScale(glm::vec3(1920.0f / 2, 1080.0f / 2, 1.0f));

		auto mre2 = elden_ring2->addComponent<MeshRenderer>();
		auto elden_ring2_material = new Material("Unlit/Texture");
		elden_ring2_material->addUniform<UniformTex2D>(ShaderParam::TexColor, "eldenring2");
		mre2->setMaterial(elden_ring2_material);
		mre2->setMesh("Square");
	}


	//----------------------------------------
	//
	//	Plane
	//
	if (true)
	{
		auto plane = new GameObject();
		plane->addComponent<Transform>();
		plane->getTransform()->setPosition(glm::vec3(0.0f, -20.0f, 0.0f));
		plane->getTransform()->setRotation(glm::vec3(0.0f, 0.0f, 0.0f));
		plane->getTransform()->setScale(glm::vec3(1.0f, 1.0f, 1.0f));

		auto mr = plane->addComponent<MeshRenderer>();
		//Standard/Std2 error!!!
		auto plane_material = new Material("Standard/Std1");
		plane_material->addUniform<UniformTex2D>(ShaderParam::StdMaterial::Diffuse, "stone_wall_diff");
		plane_material->addUniform<UniformTex2D>(ShaderParam::StdMaterial::Specular, "stone_wall_ao");
		plane_material->addUniform<UniformF1>(ShaderParam::StdMaterial::Shininess, 64.0f);
		plane_material->addUniform<UniformTex2D>(ShaderParam::TexDepth, "Shadow");
		mr->setMaterial(plane_material);
		mr->setMesh("Plane");
	}

	//----------------------------------------
	//
	//	Transparent
	//
	if (false)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(-2000, 2000);

		for (int i = 0; i < 20; i++)
		{
			auto go = new GameObject();
			go->addComponent<Transform>();
			go->getTransform()->setPosition(glm::vec3(dis(gen) / 100.0f, dis(gen) / 100.0f, dis(gen) / 100.0f));
			go->getTransform()->setScale(glm::vec3(3.0f, 3.0f, 1.0f));

			auto mr = go->addComponent<MeshRenderer>();
			auto material = new Material("Unlit/Transparent");
			material->addUniform<UniformTex2D>(ShaderParam::TexColor, "transparent_window");
			mr->setMaterial(material);
			mr->setMesh("Square");
		}
	}

	//----------------------------------------
	//
	//	Cubes 0
	//
	if (true)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(-520, 520);
		std::uniform_int_distribution<> dis_ro(0, 359);

		for (int i = 0; i < 1000; i++)
		{
			auto go = new GameObject();
			go->addComponent<Transform>();
			go->getTransform()->setPosition(glm::vec3(dis(gen), dis(gen), dis(gen)));
			go->getTransform()->setScale(glm::vec3(10.0f));
			go->getTransform()->setRotation(glm::vec3(dis_ro(gen), dis_ro(gen), dis_ro(gen)));

			auto mr = go->addComponent<MeshRenderer>();
			auto material = new Material("Standard/Std1");
			material->addUniform<UniformTex2D>(ShaderParam::StdMaterial::Diffuse, "metal_plate_diff");
			material->addUniform<UniformTex2D>(ShaderParam::StdMaterial::Specular, "metal_plate_spec");
			material->addUniform<UniformF1>(ShaderParam::StdMaterial::Shininess, 64.0f);
			material->addUniform<UniformTexCube>(ShaderParam::TexCube, "skybox_2");
			material->addUniform<UniformTex2D>(ShaderParam::TexDepth, "Shadow");
			mr->setMaterial(material);
			mr->setMesh("Cube");
			//mr->setMesh("Sphere");
		}
	}

	//----------------------------------------
	//
	//	Cubes 1
	//
	if (false)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(-1000, 1000);

		for (int i = 0; i < 1000; i++)
		{
			auto go = new GameObject();
			go->setLayerMaskIndex(1);
			go->addComponent<Transform>();
			go->getTransform()->setPosition(glm::vec3(dis(gen) / 10.0f, dis(gen) / 10.0f, dis(gen) / 10.0f));

			auto mr = go->addComponent<MeshRenderer>();
			auto material = new Material("Standard/Std1");
			material->addUniform<UniformTex2D>(ShaderParam::StdMaterial::Diffuse, "metal_plate_diff");
			material->addUniform<UniformTex2D>(ShaderParam::StdMaterial::Specular, "metal_plate_spec");
			material->addUniform<UniformF1>(ShaderParam::StdMaterial::Shininess, 16.0f);
			material->addUniform<UniformTexCube>(ShaderParam::TexCube, "skybox_2");
			mr->setMaterial(material);
			mr->setMesh("Cube");
		}
	}


	SG<InputSystem>::getInstance()->push(MyInputer::getInstance());
	this->initGUI();
}

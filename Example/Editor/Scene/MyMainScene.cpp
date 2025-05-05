#include "MyMainScene.h"
#include "../EditorInputer.h"
//#include "../MyController.h"

using namespace tezcat::Editor;

TINY_OBJECT_CPP(MyMainScene, Scene);

MyMainScene::MyMainScene(const std::string& name)
	: Base(name)
	, mController(nullptr)
{

}

void MyMainScene::onEnter()
{
	Base::onEnter();
	LightingManager::loadSkyboxHDR("Resource/Image/SkyboxHDR/newport_loft.hdr");
	InputSys::getInstance()->push(EditorInputer::getInstance());

	if (true)
	{
		auto go = GameObject::create("World1_Camera");
		auto camera = go->addComponent<Camera>();
		camera->setMain();
		camera->setViewRect(0, 0, EngineContext::ScreenWidth, EngineContext::ScreenHeight);
		camera->setPerspective(60.0f, 0.1f, 2000.0f);
		camera->setCullLayer(0);
		camera->setClearOption(ClearOption::CO_Skybox | ClearOption::CO_Depth | ClearOption::CO_Color);
		camera->setFrameBuffer(FrameBufferManager::getMainFrameBufferBuildin());

		go->addComponent<Transform>();
		go->getTransform()->setPosition(float3(-41.715f, 174.206f, 382.644f));
		go->getTransform()->setRotation(float3(-13.093f, -27.649f, 0.0f));

		go->addComponent<FlyController>();
	}

	//this->createInfinitePlane();
	this->createDirectionLight();
	this->createPaintings();
	this->createPlane();
	this->createTransparentObject();
	this->createPBR();
	this->createModel();
	this->createCubes0();

	quaternion rotation1(glm::radians(float3(30.0f, 0.0f, 0.0f)));
	quaternion rotation2(glm::radians(float3(60.0f, 0.0f, 0.0f)));

	auto rot1 = glm::degrees(glm::eulerAngles(rotation2 * rotation1));

	auto rot_t = glm::inverse(rotation1 * rotation2) * quaternion(glm::radians(rot1));
	rot_t = glm::normalize(rot_t);
	auto rot2 = glm::degrees(glm::eulerAngles(rot_t));
}

void MyMainScene::onExit()
{
	Base::onExit();
}

void MyMainScene::createModel()
{
	auto model = ResourceManager::loadAndSave<Model>("Resource/Model/szb.fbx");
	auto go_gundum_szb = model->generate();
	go_gundum_szb->getTransform()->setPosition(157.74f, 106.560f, 150.780f);
	go_gundum_szb->getTransform()->setRotation(0.0f, 0.0f, 0.0f);
	go_gundum_szb->getTransform()->setScale(50.0f, 50.0f, 50.0f);

	model = ResourceManager::loadAndSave<Model>("Resource/Model/NekomiyaMana.pmx");
	auto go_dasd = model->generate();
	go_dasd->getTransform()->setPosition(0.0f, 0.0f, 90.0f);
	go_dasd->getTransform()->setScale(10.0f, 10.0f, 10.0f);
}

void MyMainScene::createGates(float gateWidth, float gateHigh)
{
	// 	float gateWidth = 1920.0f / 4;
	// 	float gateHigh = 1080.0f / 4;

	auto world1 = GameObject::create("World1_Gate");
	world1->setLayerMaskIndex(1);

	auto tran1 = world1->addComponent<Transform>();
	tran1->setPosition(float3(200.0f, 0.0f, 0.0f));
	tran1->setRotation(float3(0.0f, -90.0f, 0.0f));
	tran1->setScale(float3(gateWidth, gateHigh, 1.0f));

	auto mr1 = world1->addComponent<MeshRenderer>();
	mr1->setMesh("Square");

	auto world1_material = Material::create("Unlit/Texture");
	mr1->setMaterial(world1_material);

	auto shader = world1_material->getShader();
	auto tex = (Texture2D*)TextureManager::find("RB_World1");
	auto index = shader->getUserUniformIndex("myTexColor2D");
	world1_material->setUniform<UniformTex2D>(index, tex);


	auto world2 = GameObject::create("World2_Gate");
	auto tran2 = world2->addComponent<Transform>();
	tran2->setPosition(float3(-300.0f, 0.0f, 0.0f));
	tran2->setRotation(float3(0.0f, 90.0f, 0.0f));
	tran2->setScale(float3(gateWidth, gateHigh, 1.0f));

	auto mr2 = world2->addComponent<MeshRenderer>();
	mr2->setMesh("Square");

	auto world2_material = Material::create("Unlit/Texture");
	mr2->setMaterial(world2_material);

	tex = (Texture2D*)TextureManager::find("RB_World2");
	index = shader->getUserUniformIndex("myTexColor2D");
	world2_material->setUniform<UniformTex2D>(index, tex);
}

void MyMainScene::createPaintings()
{
	auto go = GameObject::create("Paintings");
	auto transform = go->addComponent<Transform>();
	transform->setPosition(float3(20.0f, 0.0f, -20.0f));

	//-------------------------------------------------
	auto wife = GameObject::create("Wife");
	wife->addComponent<Transform>();
	wife->getTransform()->setPosition(float3(-960.0f, 0.0f, 0.0f));
	wife->getTransform()->setRotation(float3(0.0f, 90.0f, 0.0f));
	wife->getTransform()->setScale(float3(1920.0f / 2, 1080.0f / 2, 1.0f));
	wife->getTransform()->setParent(transform);

	auto mr2 = wife->addComponent<MeshRenderer>();
	mr2->setMesh("Square");

	auto wife_material2 = Material::create("Unlit/Texture");
	auto shader = wife_material2->getShader();
	auto my_tex2d_color_index = shader->getUserUniformIndex("myTexColor2D");
	auto tex = ResourceManager::loadOnly<Texture2D>("Resource/Image/wife.jpg");
	wife_material2->setUniform<UniformTex2D>(my_tex2d_color_index, tex);
	mr2->setMaterial(wife_material2);


	if (true)
	{
		//--------------------------------------
		auto elden_ring1 = GameObject::create("ED1");
		elden_ring1->addComponent<Transform>();
		elden_ring1->getTransform()->setPosition(float3(0.0f, 0.0f, -960.0f));
		elden_ring1->getTransform()->setRotation(float3(0.0f, 0.0f, 0.0f));
		elden_ring1->getTransform()->setScale(float3(1920.0f / 2, 1080.0f / 2, 1.0f));
		elden_ring1->getTransform()->setParent(transform);

		auto mre1 = elden_ring1->addComponent<MeshRenderer>();
		auto elden_ring1_material = Material::create("Unlit/Texture");
		mre1->setMaterial(elden_ring1_material);
		mre1->setMesh("Square");

		tex = ResourceManager::loadOnly<Texture2D>("Resource/Image/eldenring1.jpg");
		elden_ring1_material->setUniform<UniformTex2D>(my_tex2d_color_index, tex);

		//--------------------------------------
		auto elden_ring2 = GameObject::create("ED2");
		elden_ring2->addComponent<Transform>();
		elden_ring2->getTransform()->setPosition(float3(0.0f, 0.0f, 960.0f));
		elden_ring2->getTransform()->setRotation(float3(0.0f, -180.0f, 0.0f));
		elden_ring2->getTransform()->setScale(float3(1920.0f / 2, 1080.0f / 2, 1.0f));
		elden_ring2->getTransform()->setParent(transform);

		auto mre2 = elden_ring2->addComponent<MeshRenderer>();
		auto elden_ring2_material = Material::create("Unlit/Texture");
		mre2->setMaterial(elden_ring2_material);
		mre2->setMesh("Square");

		tex = ResourceManager::loadOnly<Texture2D>("Resource/Image/eldenring2.jpg");
		elden_ring2_material->setUniform<UniformTex2D>(my_tex2d_color_index, tex);

		//--------------------------------------
		auto img = ResourceManager::loadOnly<Image>("Resource/Image/SkyboxHDR/newport_loft.hdr");
		auto hdr = Texture2D::create();
		hdr->setImage(img);
		hdr->setMinFilter(TextureFilter::Linear_Mipmap_Linear);
		hdr->generate();

		auto gaussian = GameObject::create("Gaussian");
		gaussian->addComponent<Transform>();
		gaussian->getTransform()->setPosition(float3(960.0f, 0.0f, 0.0f));
		gaussian->getTransform()->setRotation(float3(0.0f, -90.0f, 0.0f));
		gaussian->getTransform()->setScale(float3(hdr->getWidth() / 2, hdr->getHeight() / 2, 1.0f));
		gaussian->getTransform()->setParent(transform);

		auto mg = gaussian->addComponent<MeshRenderer>();
		auto gaussian_material = Material::create("Unlit/GaussianBlur");
		mg->setMaterial(gaussian_material);
		mg->setMesh("Square");

		shader = gaussian_material->getShader();
		my_tex2d_color_index = shader->getUserUniformIndex("myTexColor2D");
		gaussian_material->setUniform<UniformTex2D>(my_tex2d_color_index, hdr);
	}
}

void MyMainScene::createPlane()
{
	auto plane = GameObject::create("Plane");
	plane->addComponent<Transform>();
	plane->getTransform()->setPosition(float3(0.0f, -50.0f, 0.0f));
	plane->getTransform()->setRotation(float3(0.0f, 0.0f, 0.0f));
	plane->getTransform()->setScale(float3(1.0f, 1.0f, 1.0f));

	auto mr = plane->addComponent<MeshRenderer>();
	mr->setMesh("Plane");

	auto plane_material = Material::create("Standard/Std1");
	mr->setMaterial(plane_material);

	auto shader = plane_material->getShader();
	auto index_diffuse = shader->getUserUniformIndex("myTexDiffuse2D");
	auto index_specular = shader->getUserUniformIndex("myTexSpecular2D");
	auto index_shininess = shader->getUserUniformIndex("myTexShininess2D");

	auto tex_diff = ResourceManager::loadOnly<Texture2D>("Resource/Image/stone_wall_diff.jpg");
	auto tex_spec = ResourceManager::loadOnly<Texture2D>("Resource/Image/stone_wall_ao.jpg");

	plane_material->setUniform<UniformTex2D>(index_diffuse, tex_diff);
	plane_material->setUniform<UniformTex2D>(index_specular, tex_spec);
	plane_material->setUniform<UniformTex2D>(index_shininess, tex_spec);


	//plane_material->addUniform<UniformTex2D>(ShaderParam::TexDepth, "Shadow");

}

void MyMainScene::createCubes0()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(-200, 200);
	std::uniform_int_distribution<> dis_ro(0, 359);

	auto shader = ShaderManager::find("Standard/Std1");

	auto index_diffuse = shader->getUserUniformIndex("myTexDiffuse2D");
	auto index_specular = shader->getUserUniformIndex("myTexSpecular2D");
	auto index_shininess = shader->getUserUniformIndex("myTexShininess2D");

	auto tex_diff = ResourceManager::loadOnly<Texture2D>("Resource/Image/metal_plate_diff.jpg");
	auto tex_spec = ResourceManager::loadOnly<Texture2D>("Resource/Image/metal_plate_spec.jpg");

	for (int i = 0; i < 100; i++)
	{
		auto go = GameObject::create();
		go->addComponent<Transform>();
		go->getTransform()->setPosition(float3(dis(gen), dis(gen), dis(gen)));
		go->getTransform()->setScale(float3(10.0f));
		go->getTransform()->setRotation(float3(dis_ro(gen), dis_ro(gen), dis_ro(gen)));

		auto mr = go->addComponent<MeshRenderer>();
		mr->setMesh("Cube");

		auto material = Material::create(shader);
		mr->setMaterial(material);

		material->setUniform<UniformTex2D>(index_diffuse, tex_diff);
		material->setUniform<UniformTex2D>(index_specular, tex_spec);
		//material->setUniform<UniformF1>(index_shininess, 64.0f);
		material->setUniform<UniformTex2D>(index_shininess, tex_spec);
	}
}

void MyMainScene::createTransparentObject()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(-300, 300);

	auto go = GameObject::create("Transparent");
	auto transform = go->addComponent<Transform>();
	transform->setPosition(float3(0.0f, 0.0f, -20.0f));

	for (int i = 0; i < 20; i++)
	{
		auto go = GameObject::create(StringTool::stringFormat("T%d", i));
		go->addComponent<Transform>();
		go->getTransform()->setPosition(float3(dis(gen), dis(gen), dis(gen)));
		go->getTransform()->setScale(float3(30.0f, 30.0f, 1.0f));
		go->getTransform()->setParent(transform);

		auto mr = go->addComponent<MeshRenderer>();
		mr->setMesh("Square");

		auto material = Material::create("Unlit/Transparent");
		mr->setMaterial(material);

		auto shader = material->getShader();
		auto index_color = shader->getUserUniformIndex("myTexColor2D");
		auto tex = ResourceManager::loadOnly<Texture2D>("Resource/Image/transparent_window.png");
		material->setUniform<UniformTex2D>(index_color, tex);
	}
}

void MyMainScene::createInfinitePlane()
{
	Log::info("createInfinitePlane");
	auto go = GameObject::create("InfinitePlane");
	go->addComponent<Transform>()->setPosition(0.0f, 0.0f, 0.0f);
	auto mr = go->addComponent<MeshRenderer>();
	mr->setShadowReciever(false);
	mr->setMesh("GridSquare");
	auto material = Material::create("Utility/InfiniteGrid");
	mr->setMaterial(material);
}

void MyMainScene::createDirectionLight()
{
	Log::info("createDirectionLight");
	auto go = GameObject::create("DirectionLight");
	go->addComponent<Transform>();
	go->getTransform()->setPosition(float3(0.0f, 600.0f, 600.0f));
	go->getTransform()->setScale(float3(100.0f));
	go->getTransform()->setRotation(-60.0f, 0.0f, 0.0f);

	//auto mr = go->addComponent<MeshRenderer>();
	//mr->setMesh("Sphere");
	//
	//auto light_material = Material::create("Unlit/Color");
	//mr->setMaterial(light_material);

	auto dir_light = go->addComponent<DirectionalLight>();
	dir_light->setDiffuse(float3(1.0f, 1.0f, 1.0f));
	dir_light->setAmbient(float3(0.1f));
	dir_light->setSpecular(float3(0.5f));

	auto shadow_caster = go->addComponent<ShadowCaster>();
	shadow_caster->setOrtho(0.1f, 2000.0f);
	shadow_caster->setViewRect(0, 0, 1024, 1024);
	shadow_caster->setCullLayer(0);
	shadow_caster->setShadowMap(4096, 4096, "Shadow");

	// 	dir_light->startLogic([=]()
	// 	{
	// 		go->getTransform()->rotate(float3(0.0f, 10.0f * EngineContext::DeltaTime, 0.0f));
	// 	});
}

void MyMainScene::createPBR()
{
	Log::info("createPBR");
	auto go = GameObject::create("PBR");
	auto transform = go->addComponent<Transform>();
	transform->setPosition(float3(0.0f, 0.0f, -20.0f));

	auto shader = ShaderManager::find("Standard/PBRTest1");
	auto index_albedo = shader->getUserUniformIndex("myPBR.albedo");
	auto index_metallic = shader->getUserUniformIndex("myPBR.metallic");
	auto index_roughness = shader->getUserUniformIndex("myPBR.roughness");
	auto index_ao = shader->getUserUniformIndex("myPBR.ao");


	//go = GameObject::create("PBRBall_Test");
	//go->addComponent<Transform>();
	//go->getTransform()->setPosition(float3(0.0f));
	//go->getTransform()->setScale(float3(10.0f));
	//go->getTransform()->setRotation(float3(90.0f, 0.0f, 0.0f));
	//go->getTransform()->setParent(transform);
	//
	//auto mr = go->addComponent<MeshRenderer>();
	//auto material = Material::create(shader);
	//material->setUniform<UniformF3>(index_albedo, float3(1.0f, 1.0f, 1.0f));
	//material->setUniform<UniformF1>(index_metallic, 0.5f);
	//material->setUniform<UniformF1>(index_roughness, 0.5f);
	//material->setUniform<UniformF1>(index_ao, 1.0f);
	//mr->setMaterial(material);
	//mr->setMesh("Sphere");


	if (true)
	{
		int number = 5;
		float rate = 1.0f / (float)number;
		for (int y = 0; y <= number; y++)
		{
			for (int x = 0; x <= number; x++)
			{
				go = GameObject::create(std::format("PBRBall_{}_{}", x, y));
				go->addComponent<Transform>();
				go->getTransform()->setPosition(float3(x * 32.0f, y * 32.0f, 0.0f));
				go->getTransform()->setScale(float3(30.0f));
				//go->getTransform()->setRotation(float3(90.0f, 0.0f, 0.0f));
				go->getTransform()->setParent(transform);

				auto mr = go->addComponent<MeshRenderer>();
				auto material = Material::create(shader);
				material->setUniform<UniformF3>(index_albedo, float3(1.0f, 1.0f, 1.0f));
				material->setUniform<UniformF1>(index_metallic, x * rate);
				material->setUniform<UniformF1>(index_roughness, y * rate);
				material->setUniform<UniformF1>(index_ao, 1.0f);
				mr->setMaterial(material);
				mr->setMesh("Sphere");
			}
		}
	}
}

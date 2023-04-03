#include "MyScene.h"
#include "MyInputer.h"
#include "MyGUI.h"


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

	{
		auto go = new GameObject("MainCamera");
		auto camera = go->addComponent<Camera>(true);
		camera->setPerspective(60.0f, 0.1f, 2000.0f);
		camera->setCullLayer(0);

		go->addComponent<Transform>();
		go->getTransform()->setPosition(glm::vec3(0.0f, 0.0f, 10.0f));
	}

	{
		auto direction_light_go = new GameObject();
		direction_light_go->addComponent<Transform>();
		direction_light_go->getTransform()->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		direction_light_go->getTransform()->setScale(glm::vec3(4.0f));

		auto mr = direction_light_go->addComponent<MeshRenderer>();
		auto light_material = new Material("Unlit/Color");
		mr->setMaterial(light_material);
		mr->setMesh("Cube");

		auto dir_light = direction_light_go->addComponent<DirectionalLight>();
		dir_light->setDirection(glm::vec3(0.0f, -1.0f, 0.0f));
		dir_light->setDiffuse(glm::vec3(1.0f, 0.0f, 0.0f));
		dir_light->setAmbient(glm::vec3(0.1f));
		dir_light->setSpecular(glm::vec3(0.5f));
	}

	{
		auto wife1 = new GameObject();
		auto tran = wife1->addComponent<Transform>();
		tran->setPosition(glm::vec3(-960.0f, 0.0f, 0.0f));
		tran->setRotation(glm::vec3(0.0f, 90.0f, 0.0f));
		tran->setScale(glm::vec3(1920.0f, 1080.0f, 1.0f));

		auto mr1 = wife1->addComponent<MeshRenderer>();
		auto wife1_material = new Material("Unlit/Texture");
		wife1_material->addUniform<UniformTex2D>(ShaderParam::TexColor, "../Resource/Image/reimu.jpg");
		mr1->setMaterial(wife1_material);
		mr1->setMesh("Square");


		//-------------------------------------------------
		auto wife2 = new GameObject();
		wife2->addComponent<Transform>();
		wife2->getTransform()->setPosition(glm::vec3(960.0f, 0.0f, 0.0f));
		wife2->getTransform()->setRotation(glm::vec3(0.0f, -90.0f, 0.0f));
		wife2->getTransform()->setScale(glm::vec3(1920.0f, 1080.0f, 1.0f));

		auto mr2 = wife2->addComponent<MeshRenderer>();
		auto wife_material2 = new Material("Unlit/Texture");
		wife_material2->addUniform<UniformTex2D>(ShaderParam::TexColor, "../Resource/Image/wifu.jpg");
		mr2->setMaterial(wife_material2);
		mr2->setMesh("Square");


		//--------------------------------------
		auto elden_ring1 = new GameObject();
		elden_ring1->addComponent<Transform>();
		elden_ring1->getTransform()->setPosition(glm::vec3(0.0f, 0.0f, -960.0f));
		elden_ring1->getTransform()->setRotation(glm::vec3(0.0f, 0.0f, 0.0f));
		elden_ring1->getTransform()->setScale(glm::vec3(1920.0f, 1080.0f, 1.0f));

		auto mre1 = elden_ring1->addComponent<MeshRenderer>();
		auto elden_ring1_material = new Material("Unlit/Texture");
		elden_ring1_material->addUniform<UniformTex2D>(ShaderParam::TexColor, "../Resource/Image/eldenring1.jpg");
		mre1->setMaterial(elden_ring1_material);
		mre1->setMesh("Square");


		auto elden_ring2 = new GameObject();
		elden_ring2->addComponent<Transform>();
		elden_ring2->getTransform()->setPosition(glm::vec3(0.0f, 0.0f, 960.0f));
		elden_ring2->getTransform()->setRotation(glm::vec3(0.0f, -180.0f, 0.0f));
		elden_ring2->getTransform()->setScale(glm::vec3(1920.0f, 1080.0f, 1.0f));

		auto mre2 = elden_ring2->addComponent<MeshRenderer>();
		auto elden_ring2_material = new Material("Unlit/Texture");
		elden_ring2_material->addUniform<UniformTex2D>(ShaderParam::TexColor, "../Resource/Image/eldenring2.jpg");
		mre2->setMaterial(elden_ring2_material);
		mre2->setMesh("Square");
	}

	{
		if (true)
		{
			auto plane = new GameObject();
			plane->addComponent<Transform>();
			plane->getTransform()->setPosition(glm::vec3(0.0f, -20.0f, 0.0f));
			plane->getTransform()->setRotation(glm::vec3(-90.0f, 0.0f, 0.0f));
			plane->getTransform()->setScale(glm::vec3(20.0f, 20.0f, 1.0f));

			auto mr = plane->addComponent<MeshRenderer>();
			//Standard/Std2 error!!!
			auto plane_material = new Material("Standard/Std1");
			plane_material->addUniform<UniformTex2D>(ShaderParam::StdMaterial::Diffuse, "../Resource/Image/container.png");
			plane_material->addUniform<UniformTex2D>(ShaderParam::StdMaterial::Specular, "../Resource/Image/container_specular.png");
			plane_material->addUniform<UniformF1>(ShaderParam::StdMaterial::Shininess, 64.0f);
			mr->setMaterial(plane_material);
			mr->setMesh("Square");
		}
	}

	{
		if (true)
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<> dis(-900, 900);

			for (int i = 0; i < 1000; i++)
			{
				auto go = new GameObject();
				go->addComponent<Transform>();
				go->getTransform()->setPosition(glm::vec3(dis(gen) / 10.0f, dis(gen) / 10.0f, dis(gen) / 10.0f));

//				Standard/Std2 error!!!
//  			auto material = new Material("Standard/Std2");
// 				material->addUniform<UniformTex2D>(ShaderParam::StdMaterial::Diffuse, "../Resource/Image/container.png");
// 				material->addUniform<UniformTex2D>(ShaderParam::StdMaterial::Specular, "../Resource/Image/container_specular.png");
// 				material->addUniform<UniformF>(ShaderParam::StdMaterial::Shininess, 64.0f);

				auto mr = go->addComponent<MeshRenderer>();
				auto material = new Material("Standard/Std1");
				material->addUniform<UniformTex2D>(ShaderParam::TexColor, "../Resource/Image/dragon.jpg");
				material->addUniform<UniformF1>(ShaderParam::ModeSpecular, 64.0f);
				mr->setMaterial(material);
				mr->setMesh("Cube");
			}
		}
	}

	SG<InputSystem>::getInstance()->push(MyInputer::getInstance());

	this->initGUI();
}

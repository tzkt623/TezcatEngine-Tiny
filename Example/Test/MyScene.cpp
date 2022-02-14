#include "MyScene.h"
#include "MyInputer.h"
#include "MyGUI.h"
#include <random>



MyScene::MyScene(const std::string& name)
	: Scene(name)
{

}

void MyScene::onEnter()
{
	Scene::onEnter();

	{
		auto go = new GameObject("MainCamera");
		auto camera = go->addComponent<Camera>(true);
		camera->setPerspective(45.0f, 0.1f, 2000.0f);
		go->getTransform()->setPosition(glm::vec3(0.0f, 0.0f, 10.0f));
		this->addGameObject(go);

		// 		go->startLogic(
		// 			[=]()
		// 			{
		// 				camera->yawPitch(Engine::getDeltaTime() * 100, 0);
		// 			});
	}

	{
		auto light_material = new Material("Unlit/Color");

		auto light = new GameObject();
		auto mr = light->addComponent<MeshRenderer>();
		mr->setMaterial(light_material);
		mr->setMesh("Cube");
		light->getTransform()->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		light->getTransform()->setScale(glm::vec3(4.0f));
		this->addGameObject(light);
	}

	{
		auto texture = new Texture();
		texture->createTexture("../Resource/Image/reimu.jpg");

		auto wife1_material = new Material("Unlit/Texture");
		wife1_material->addTexture(TINY_TexColor, texture);

		auto wife1 = new GameObject();
		auto mr1 = wife1->addComponent<MeshRenderer>();
		mr1->setMaterial(wife1_material);
		mr1->setMesh("Square");
		wife1->getTransform()->setPosition(glm::vec3(-960.0f, 0.0f, 0.0f));
		wife1->getTransform()->setRotation(glm::vec3(0.0f, 90.0f, 0.0f));
		wife1->getTransform()->setScale(glm::vec3(1920.0f, 1080.0f, 1));
		this->addGameObject(wife1);


		auto texture2 = new Texture();
		texture2->createTexture("../Resource/Image/wifu.jpg");

		auto wife_material2 = new Material("Unlit/Texture");
		wife_material2->addTexture(TINY_TexColor, texture2);

		auto wife2 = new GameObject();
		auto mr2 = wife2->addComponent<MeshRenderer>();
		mr2->setMaterial(wife_material2);
		mr2->setMesh("Square");
		wife2->getTransform()->setPosition(glm::vec3(960.0f, 0.0f, 0.0f));
		wife2->getTransform()->setRotation(glm::vec3(0.0f, -90.0f, 0.0f));
		wife2->getTransform()->setScale(glm::vec3(1920.0f, 1080.0f, 1));
		this->addGameObject(wife2);


		//--------------------------------------

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
	}

	{
		auto texture = new Texture();
		texture->createTexture("../Resource/Image/container.png");

		auto plane_material = new Material("Standard/Std1");
		plane_material->addTexture(TINY_TexColor, texture);

		auto plane = new GameObject();

		auto mr = plane->addComponent<MeshRenderer>();
		mr->setMaterial(plane_material);
		mr->setMesh("Square");

		plane->getTransform()->setPosition(glm::vec3(0.0f, -20.0f, 0.0f));
		plane->getTransform()->setRotation(glm::vec3(-90.0f, 0.0f, 0.0f));
		plane->getTransform()->setScale(glm::vec3(20.0f, 20.0f, 1));
		this->addGameObject(plane);

	}

	{
		auto texture = new Texture();
		texture->createTexture("../Resource/Image/dragon.jpg");

		auto material = new Material("Standard/Std1");
		material->addTexture(TINY_TexColor, texture);

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(-900, 900);

		for (int i = 0; i < 1000; i++)
		{
			auto go = new GameObject();

			auto mr = go->addComponent<MeshRenderer>();
			mr->setMaterial(material);
			mr->setMesh("Cube");

			go->getTransform()->setPosition(glm::vec3(dis(gen) / 10.0f, dis(gen) / 10.0f, dis(gen) / 10.0f));
			this->addGameObject(go);
		}
	}

	SG<InputSystem>::getInstance()->push(MyInputer::getInstance());

	this->initGUI();
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

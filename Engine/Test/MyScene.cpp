#include "MyScene.h"
#include "MyInputer.h"
#include "MyGUI.h"
#include <random>



MyScene::MyScene(const std::string& name) :
	Scene(name)
{

}

void MyScene::onEnter()
{
	Scene::onEnter();

	auto camera = new Camera("Main");
	camera->setPerspective(45.0f, 0.1f, 100.0f);
	camera->getTransform()->setPosition(glm::vec3(0.0f, 0.0f, 10.0f));
	this->addGameObject(camera);

	Image image;
	image.openFile("../Resource/Image/dragon.jpg");

	auto texture = new Texture();
	texture->createTexture(&image);

	auto material = new Material("Standard/Std1");
	material->addTexture(TINY_TexColor, texture);

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(-900, 900);

	for (int i = 0; i < 1000; i++)
	{
		auto go = new GameObject();

		auto agent = go->getRenderAgent();
		agent->setMaterial(material);
		agent->setMesh("Cube");

		go->getTransform()->setPosition(glm::vec3(dis(gen) / 10.0f, dis(gen) / 10.0f, dis(gen) / 10.0f));
		go->apply();

		this->addGameObject(go);
	}

	InputSystem::getInstance()->push(MyInputer::getInstace());

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

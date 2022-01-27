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

	auto mesh = new Mesh();
	//前
	mesh->vertices.emplace_back(0.5f, 0.5f, 0.5f);
	mesh->vertices.emplace_back(0.5f, -0.5f, 0.5f);
	mesh->vertices.emplace_back(-0.5f, -0.5f, 0.5f);
	mesh->vertices.emplace_back(-0.5f, 0.5f, 0.5f);

	//上
	mesh->vertices.emplace_back(-0.5f, 0.5f, 0.5f);
	mesh->vertices.emplace_back(-0.5f, 0.5f, -0.5f);
	mesh->vertices.emplace_back(0.5f, 0.5f, -0.5f);
	mesh->vertices.emplace_back(0.5f, 0.5f, 0.5f);

	//后
	mesh->vertices.emplace_back(0.5f, 0.5f, -0.5f);
	mesh->vertices.emplace_back(-0.5f, 0.5f, -0.5f);
	mesh->vertices.emplace_back(-0.5f, -0.5f, -0.5f);
	mesh->vertices.emplace_back(0.5f, -0.5f, -0.5f);

	//下
	mesh->vertices.emplace_back(-0.5f, -0.5f, 0.5f);
	mesh->vertices.emplace_back(0.5f, -0.5f, 0.5f);
	mesh->vertices.emplace_back(0.5f, -0.5f, -0.5f);
	mesh->vertices.emplace_back(-0.5f, -0.5f, -0.5f);

	//左
	mesh->vertices.emplace_back(-0.5f, 0.5f, -0.5f);
	mesh->vertices.emplace_back(-0.5f, 0.5f, 0.5f);
	mesh->vertices.emplace_back(-0.5f, -0.5f, 0.5f);
	mesh->vertices.emplace_back(-0.5f, -0.5f, -0.5f);

	//右
	mesh->vertices.emplace_back(0.5f, 0.5f, 0.5f);
	mesh->vertices.emplace_back(0.5f, 0.5f, -0.5f);
	mesh->vertices.emplace_back(0.5f, -0.5f, -0.5f);
	mesh->vertices.emplace_back(0.5f, -0.5f, 0.5f);

	//前
	for (int i = 0; i < 6; i++)
	{
		int offset = i * 4;
		mesh->indices.emplace_back(offset + 0);
		mesh->indices.emplace_back(offset + 1);
		mesh->indices.emplace_back(offset + 3);

		mesh->indices.emplace_back(offset + 1);
		mesh->indices.emplace_back(offset + 2);
		mesh->indices.emplace_back(offset + 3);

		mesh->colors.emplace_back(1.0f, 0.0f, 0.0f, 1.0f);
		mesh->colors.emplace_back(0.0f, 1.0f, 0.0f, 1.0f);
		mesh->colors.emplace_back(0.0f, 0.0f, 1.0f, 1.0f);
		mesh->colors.emplace_back(1.0f, 0.0f, 1.0f, 1.0f);

		mesh->uv.emplace_back(1.0f, 1.0f);
		mesh->uv.emplace_back(1.0f, 0.0f);
		mesh->uv.emplace_back(0.0f, 0.0f);
		mesh->uv.emplace_back(0.0f, 1.0f);
	}


	mesh->apply();

	Image image;
	image.openFile("../Resource/Image/dragon.jpg");

	auto texture = new Texture();
	texture->createTexture(&image);

	auto material = new Material("Default");
	material->addTexture("texColor", texture);

	/*
		auto go = new GameObject();
		go->setMaterial(material);
		go->setMesh(mesh);
		go->getTransform()->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));

		auto go2 = new GameObject();
		go2->setMaterial(material);
		go2->setMesh(mesh);
		go2->getTransform()->setPosition(glm::vec3(1.0f, 0.0f, 0.0f));
		go2->getTransform()->setRotation(glm::vec3(0.0f, 0.0f, 45.0f));

		auto go3 = new GameObject();
		go3->setMaterial(material);
		go3->setMesh(mesh);
		go3->getTransform()->setPosition(glm::vec3(1.0f, 0.0f, 0.0f));
		go3->getTransform()->setRotation(glm::vec3(0.0f, 0.0f, 45.0f));

		go2->addChild(go3);
		go->addChild(go2);
		go->apply();

		this->addGameObject(go);
	*/

	float resolution = 64;
	float invResolution = 1 / resolution;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(-90.0f, 90.0f);

	for (int i = 0; i < 1000; i++)
	{
		auto go = new GameObject();
		go->setMaterial(material);
		go->setMesh(mesh);
		go->getTransform()->setPosition(glm::vec3(dis(gen), dis(gen), dis(gen)));
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

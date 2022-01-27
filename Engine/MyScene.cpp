#include "MyScene.h"
#include "MyInputer.h"
#include "MyGUI.h"


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
	mesh->vertices.emplace_back(0.5f, 0.5f, 0.0f);
	mesh->vertices.emplace_back(0.5f, -0.5f, 0.0f);
	mesh->vertices.emplace_back(-0.5f, -0.5f, 0.0f);
	mesh->vertices.emplace_back(-0.5f, 0.5f, 0.0f);

	mesh->indices.emplace_back(0);
	mesh->indices.emplace_back(1);
	mesh->indices.emplace_back(3);

	mesh->indices.emplace_back(1);
	mesh->indices.emplace_back(2);
	mesh->indices.emplace_back(3);

	mesh->colors.emplace_back(1.0f, 0.0f, 0.0f, 1.0f);
	mesh->colors.emplace_back(0.0f, 1.0f, 0.0f, 1.0f);
	mesh->colors.emplace_back(0.0f, 0.0f, 1.0f, 1.0f);
	mesh->colors.emplace_back(1.0f, 0.0f, 1.0f, 1.0f);
	mesh->apply();

	auto material = new Material("Default");

	auto go = new GameObject();
	go->setMaterial(material);
	go->setMesh(mesh);
	go->getTransform()->setPosition(glm::vec3(1.0f, 0.0f, 0.0f));
	//	go->getTransform()->setRotation(glm::vec3(0.0f, 0.0f, 45.0f));

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

	InputSystem::getInstance()->push(MyInputer::getInstace());

	this->initGUI();
}

void MyScene::initGUI()
{
	auto info = new MyInfoWindow();
	info->open();

	auto object = new MyObjectWindow();
	object->open();
}

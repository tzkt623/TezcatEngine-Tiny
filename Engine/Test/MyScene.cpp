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

	auto mesh_data = new MeshData("Cube");
	//前
	mesh_data->vertices.emplace_back(0.5f, 0.5f, 0.5f);
	mesh_data->vertices.emplace_back(0.5f, -0.5f, 0.5f);
	mesh_data->vertices.emplace_back(-0.5f, -0.5f, 0.5f);
	mesh_data->vertices.emplace_back(-0.5f, 0.5f, 0.5f);

	//上
	mesh_data->vertices.emplace_back(-0.5f, 0.5f, 0.5f);
	mesh_data->vertices.emplace_back(-0.5f, 0.5f, -0.5f);
	mesh_data->vertices.emplace_back(0.5f, 0.5f, -0.5f);
	mesh_data->vertices.emplace_back(0.5f, 0.5f, 0.5f);

	//后
	mesh_data->vertices.emplace_back(0.5f, 0.5f, -0.5f);
	mesh_data->vertices.emplace_back(-0.5f, 0.5f, -0.5f);
	mesh_data->vertices.emplace_back(-0.5f, -0.5f, -0.5f);
	mesh_data->vertices.emplace_back(0.5f, -0.5f, -0.5f);

	//下
	mesh_data->vertices.emplace_back(-0.5f, -0.5f, 0.5f);
	mesh_data->vertices.emplace_back(0.5f, -0.5f, 0.5f);
	mesh_data->vertices.emplace_back(0.5f, -0.5f, -0.5f);
	mesh_data->vertices.emplace_back(-0.5f, -0.5f, -0.5f);

	//左
	mesh_data->vertices.emplace_back(-0.5f, 0.5f, -0.5f);
	mesh_data->vertices.emplace_back(-0.5f, 0.5f, 0.5f);
	mesh_data->vertices.emplace_back(-0.5f, -0.5f, 0.5f);
	mesh_data->vertices.emplace_back(-0.5f, -0.5f, -0.5f);

	//右
	mesh_data->vertices.emplace_back(0.5f, 0.5f, 0.5f);
	mesh_data->vertices.emplace_back(0.5f, 0.5f, -0.5f);
	mesh_data->vertices.emplace_back(0.5f, -0.5f, -0.5f);
	mesh_data->vertices.emplace_back(0.5f, -0.5f, 0.5f);

	//前
	for (int i = 0; i < 6; i++)
	{
		int offset = i * 4;
		mesh_data->indices.emplace_back(offset + 0);
		mesh_data->indices.emplace_back(offset + 1);
		mesh_data->indices.emplace_back(offset + 3);

		mesh_data->indices.emplace_back(offset + 1);
		mesh_data->indices.emplace_back(offset + 2);
		mesh_data->indices.emplace_back(offset + 3);

		mesh_data->colors.emplace_back(1.0f, 1.0f, 1.0f, 1.0f);
		mesh_data->colors.emplace_back(1.0f, 1.0f, 1.0f, 1.0f);
		mesh_data->colors.emplace_back(1.0f, 1.0f, 1.0f, 1.0f);
		mesh_data->colors.emplace_back(1.0f, 1.0f, 1.0f, 1.0f);

		mesh_data->uv.emplace_back(1.0f, 1.0f);
		mesh_data->uv.emplace_back(1.0f, 0.0f);
		mesh_data->uv.emplace_back(0.0f, 0.0f);
		mesh_data->uv.emplace_back(0.0f, 1.0f);
	}

	mesh_data->apply();

	Image image;
	image.openFile("../Resource/Image/dragon.jpg");

	auto texture = new Texture();
	texture->createTexture(&image);

	auto material = new Material("Default");
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

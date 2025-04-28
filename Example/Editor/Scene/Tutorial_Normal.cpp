#include "Tutorial_Normal.h"

TINY_OBJECT_CPP(Tutorial_Normal, Scene);

Tutorial_Normal::Tutorial_Normal(const std::string& name)
	: Base(name)
{

}

Tutorial_Normal::~Tutorial_Normal()
{

}

void Tutorial_Normal::onEnter()
{
	Base::onEnter();

	GameObject* go_light = GameObject::create("DirectionLight");
	go_light->addComponent<Transform>();
	go_light->getTransform()->setPosition(float3(10.0f, 10.0f, 10.0f));
	go_light->getTransform()->setRotation(-60.0f, 0.0f, 0.0f);

	auto dir_light = go_light->addComponent<DirectionalLight>();
	dir_light->setDiffuse(float3(1.0f, 1.0f, 1.0f));
	dir_light->setAmbient(float3(0.1f));
	dir_light->setSpecular(float3(0.5f));

	GameObject* go_camera = GameObject::create("MainCamera");
	auto transform = go_camera->addComponent<Transform>();
	transform->setPosition(0.0f, 15.0f, 30.0f);
	transform->setRotation(-20.0f, 0.0f, 0.0f);

	auto camera = go_camera->addComponent<Camera>();
	camera->setPerspective(75.0f, 0.1f, 2000.0f);
	camera->setClearColor(float4(0.3f, 0.3f, 0.3f, 1.0f));
	camera->setClearOption({ ClearOption::CO_Color | ClearOption::CO_Depth });
	camera->setCullLayer(0);
	camera->setViewRect(0, 0, EngineContext::ScreenWidth, EngineContext::ScreenHeight);
	camera->setFrameBuffer(FrameBufferManager::getMainFrameBufferBuildin());


	{
		GameObject* go = GameObject::create("Plane");
		auto transform_cube = go->addComponent<Transform>();
		transform_cube->setPosition(0.0f, 0.0f, 0.0f);

		auto mr = go->addComponent<MeshRenderer>();
		mr->setMesh("Plane");

		auto material = Material::create("Tutorial/Tangent");
		mr->setMaterial(material);
	}
}

void Tutorial_Normal::onExit()
{
	Base::onExit();
}


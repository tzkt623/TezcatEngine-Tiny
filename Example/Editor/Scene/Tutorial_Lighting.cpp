#include "Tutorial_Lighting.h"

TINY_OBJECT_CPP(Tutorial_Lighting, Scene);

Tutorial_Lighting::Tutorial_Lighting(const std::string& name)
	: Base(name)
{

}

Tutorial_Lighting::~Tutorial_Lighting()
{

}

void Tutorial_Lighting::onEnter()
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
	camera->setViewRect(0, 0, Engine::getScreenWidth(), Engine::getScreenHeight());
	camera->setFrameBuffer(FrameBufferManager::getMainFrameBufferBuildin());


	{
		GameObject* go = GameObject::create("Lambert");
		auto transform_cube = go->addComponent<Transform>();
		//transform_cube->setScale(10.0f, 10.0f, 10.0f);
		transform_cube->setPosition(0.0f, 0.0f, 0.0f);

		auto mr = go->addComponent<MeshRenderer>();
		mr->setMesh("Sphere");

		auto material = Material::create("Tutorial/Lambert");
		material->setUniform<UniformF4>("myColor", float4(1.0f));
		mr->setMaterial(material);
	}

	{
		GameObject* go = GameObject::create("LambertHalf");
		auto transform_cube = go->addComponent<Transform>();
		//transform_cube->setScale(10.0f, 10.0f, 10.0f);
		transform_cube->setPosition(2.0f, 0.0f, 0.0f);

		auto mr = go->addComponent<MeshRenderer>();
		mr->setMesh("Sphere");

		auto material = Material::create("Tutorial/LambertHalf");
		material->setUniform<UniformF4>("myColor", float4(1.0f));
		mr->setMaterial(material);
	}

	{
		GameObject* go = GameObject::create("Phong");
		auto transform_cube = go->addComponent<Transform>();
		//transform_cube->setScale(10.0f, 10.0f, 10.0f);
		transform_cube->setPosition(4.0f, 0.0f, 0.0f);

		auto mr = go->addComponent<MeshRenderer>();
		mr->setMesh("Sphere");

		auto material = Material::create("Tutorial/Phong");
		material->setUniform<UniformF4>("myColor", float4(1.0f));
		material->setUniform<UniformI1>("myShininess", 32);
		mr->setMaterial(material);
	}

	{
		GameObject* go = GameObject::create("BlinnPhong");
		auto transform_cube = go->addComponent<Transform>();
		//transform_cube->setScale(10.0f, 10.0f, 10.0f);
		transform_cube->setPosition(6.0f, 0.0f, 0.0f);

		auto mr = go->addComponent<MeshRenderer>();
		mr->setMesh("Sphere");

		auto material = Material::create("Tutorial/BlinnPhong");
		material->setUniform<UniformF4>("myColor", float4(1.0f));
		material->setUniform<UniformI1>("myShininess", 32);
		mr->setMaterial(material);
	}

	{
		GameObject* go = GameObject::create("Fresnel");
		auto transform_cube = go->addComponent<Transform>();
		//transform_cube->setScale(10.0f, 10.0f, 10.0f);
		transform_cube->setPosition(0.0f, 2.0f, 0.0f);

		auto mr = go->addComponent<MeshRenderer>();
		mr->setMesh("Sphere");

		auto material = Material::create("Tutorial/Fresnel");
		material->setUniform<UniformF3>("myFresnel", float3(0.04f));
		mr->setMaterial(material);
	}
}

void Tutorial_Lighting::onExit()
{
	Base::onExit();
}


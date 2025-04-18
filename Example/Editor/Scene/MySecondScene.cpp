#include "MySecondScene.h"
#include "../EditorInputer.h"
#include "../MyController.h"
using namespace tezcat::Editor;

TINY_OBJECT_CPP(MySeconedScene, Scene);

MySeconedScene::MySeconedScene(const std::string& name)
	: Scene(name)
{

}

MySeconedScene::~MySeconedScene()
{

}

void MySeconedScene::onEnter()
{
	Base::onEnter();
	InputSys::getInstance()->push(EditorInputer::getInstance());

	if (true)
	{
		auto go = GameObject::create("World2_Camera");
		auto camera = go->addComponent<Camera>(true);
		camera->setViewRect(0, 0, Engine::getScreenWidth(), Engine::getScreenHeight());
		camera->setPerspective(60.0f, 0.1f, 2000.0f);
		camera->setCullLayer(0);
		camera->setSortingID(0);
		camera->setFrameBuffer(FrameBufferManager::getMainFrameBufferBuildin());

		EditorInputer::getInstance()->setController(go->addComponent<FlyController>());

		go->addComponent<Transform>();
		go->getTransform()->setPosition(float3(0.0f, 0.0f, 0.0f));

	}

	//----------------------------------------
	//
	//	Cubes 1
	//
	if (true)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(-1000, 1000);

		auto shader = ShaderManager::find("Standard/Std1");

		auto index_diffuse = shader->getUserUniformIndex("myTex2DDiffuse");
		auto index_specular = shader->getUserUniformIndex("myTex2DSpecular");
		auto index_shininess = shader->getUserUniformIndex("myShininess");

		auto tex_diff = ResourceManager::loadOnly<Texture2D>("Image/metal_plate_diff.jpg");
		auto tex_spec = ResourceManager::loadOnly<Texture2D>("Image/metal_plate_spec.jpg");


		for (int i = 0; i < 1000; i++)
		{
			auto go = GameObject::create();
			go->setLayerMaskIndex(0);
			go->addComponent<Transform>();
			go->getTransform()->setPosition(float3(dis(gen), dis(gen), dis(gen)));
			go->getTransform()->setScale(float3(10.0f));

			auto mr = go->addComponent<MeshRenderer>();
			mr->setMesh("Sphere");

			auto material = Material::create(shader);
			mr->setMaterial(material);

			material->setUniform<UniformTex2D>(index_diffuse, tex_diff);
			material->setUniform<UniformTex2D>(index_specular, tex_spec);
			material->setUniform<UniformF1>(index_shininess, 16.0f);
			//material->addUniform<UniformTexCube>(ShaderParam::TexCube, "skybox_2");
		}
	}
}

void MySeconedScene::onExit()
{
	Base::onExit();
}

void MySeconedScene::onPause()
{
	Base::onPause();
}

void MySeconedScene::onResume()
{
	Base::onResume();
}


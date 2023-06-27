#include "MySecondScene.h"
#include "../MyInputer.h"
#include "../MyController.h"

TINY_RTTI_CPP(MySeconedScene);

MySeconedScene::MySeconedScene(const std::string& name)
	: Scene(name)
{
}

MySeconedScene::~MySeconedScene()
{

}

void MySeconedScene::onEnter()
{
	Scene::onEnter();
	InputSys::getInstance()->push(MyInputer::getInstance());

	if (true)
	{
		auto go = GameObject::create("World2_Camera");
		auto camera = go->addComponent<Camera>(true);
		camera->setViewRect(0, 0, Engine::getScreenWidth(), Engine::getScreenHeight());
		camera->setPerspective(60.0f, 0.1f, 2000.0f);
		camera->setCullLayer(0);
		camera->setOrder(0);

		MyInputer::getInstance()->setController(go->addComponent<FlyController>());

		go->addComponent<Transform>();
		go->getTransform()->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));



		auto frame = FrameBuffer::create("FB_Viewport");

		Texture2D* t2d = Texture2D::create("RB_Viewport");
		t2d->setData(Engine::getScreenWidth(), Engine::getScreenHeight(),
			TextureInfo("RB_Viewport"
				, TextureAttachPosition::ColorComponent
				, TextureChannel::RGBA
				, TextureChannel::RGBA
				, DataType::UByte));
		frame->addAttachment(t2d);

		t2d = Texture2D::create("RB_Viewport");
		t2d->setData(Engine::getScreenWidth(), Engine::getScreenHeight(),
			TextureInfo("DS_Viewport"
				, TextureAttachPosition::DepthComponent
				, TextureChannel::Depth
				, TextureChannel::Depth
				, DataType::UByte));
		frame->addAttachment(t2d);
		frame->generate();

		camera->setFrameBuffer(frame);
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

		auto shader = ShaderMgr::getInstance()->find("Standard/Std1");

		auto index_diffuse = shader->getUniformIndex("myTex2DDiffuse");
		auto index_specular = shader->getUniformIndex("myTex2DSpecular");
		auto index_shininess = shader->getUniformIndex("myShininess");

		auto tex_diff = Resource::loadOnly<Texture2D>("Image/metal_plate_diff.jpg");
		auto tex_spec = Resource::loadOnly<Texture2D>("Image/metal_plate_spec.jpg");


		for (int i = 0; i < 1000; i++)
		{
			auto go = GameObject::create();
			go->setLayerMaskIndex(0);
			go->addComponent<Transform>();
			go->getTransform()->setPosition(glm::vec3(dis(gen), dis(gen), dis(gen)));
			go->getTransform()->setScale(glm::vec3(10.0f));

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
	Scene::onExit();
}

void MySeconedScene::onPause()
{

}

void MySeconedScene::onResume()
{

}


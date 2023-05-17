#include "Tutorial01.h"
#include "../MyObserver.h"

TINY_RTTI_CPP(Tutorial01);

Tutorial01::Tutorial01(const std::string& name)
	: Scene(name)
{

}

Tutorial01::~Tutorial01()
{

}

void Tutorial01::onEnter()
{
	Scene::onEnter();

	auto frame = FrameBufferMgr::getInstance()->create(
		"FB_Viewport",
		Engine::getScreenWidth(), Engine::getScreenHeight(),
		{
			TextureInfo("RB_Viewport"
					  , TextureAttachPosition::ColorComponent
					  , TextureChannel::RGBA
					  , TextureChannel::RGBA
					  , DataType::UByte),

			TextureInfo("DS_Viewport"
					  , TextureAttachPosition::DepthComponent
					  , TextureChannel::Depth
					  , TextureChannel::Depth
					  , DataType::UByte)
		});

	mObserver = new MyObserver();
	mObserver->setFrameBuffer(frame);
	mObserver->setViewRect(0, 0, Engine::getScreenWidth(), Engine::getScreenHeight());
	mObserver->setClearOption(ClearOption(ClearOption::CO_Color | ClearOption::CO_Depth));


	MeshData mesh;
	mesh.name = "Triangle";
	mesh.vertices.emplace_back(-0.5f, -0.5f, 0.0f);	// left
	mesh.vertices.emplace_back(0.5f, -0.5f, 0.0f);	// right
	mesh.vertices.emplace_back(0.0f, 0.5f, 0.0f);	// top
	mesh.apply();

	mVertex = BufferMgr::getInstance()->createVertex(&mesh);
	mVertex->addRef();

	auto shader = ShaderMgr::getInstance()->findPackage("Tutorial/t01")->getShaders()[0];
	auto queue = mObserver->getRenderQueue();
	this->addLogicFunction(this, [=]()
		{
			auto render_command = new RenderCMD_Vertex(mVertex, shader);
			queue->addRenderCommand(render_command);
			Graphics::getInstance()->addPreRenderPassQueue((ExtraQueue*)queue);
		});
}

void Tutorial01::onExit()
{
	Scene::onExit();
	mVertex->subRef();
	delete mObserver;
}

void Tutorial01::onPause()
{
	Scene::onPause();
}

void Tutorial01::onResume()
{
	Scene::onResume();
}

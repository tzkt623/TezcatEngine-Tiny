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

	auto frame = FrameBufferMgr::getInstance()->create("FB_Viewport");
	auto rb = TextureMgr::getInstance()->create2D("RB_Viewport");
	rb->setData(Engine::getScreenWidth()
		, Engine::getScreenHeight()
		, TextureInfo(TextureAttachPosition::ColorComponent
			, TextureChannel::RGBA
			, TextureChannel::RGBA
			, DataType::UByte));

	auto ds = TextureMgr::getInstance()->create2D("DS_Viewport");
	ds->setData(Engine::getScreenWidth()
		, Engine::getScreenHeight()
		, TextureInfo(TextureAttachPosition::DepthComponent
			, TextureChannel::Depth
			, TextureChannel::Depth
			, DataType::UByte));

	frame->addAttachment(rb);
	frame->addAttachment(ds);
	frame->generate();

	mObserver = new MyObserver();
	mObserver->setFrameBuffer(frame);
	mObserver->setViewRect(0, 0, Engine::getScreenWidth(), Engine::getScreenHeight());
	mObserver->setClearOption(ClearOption(ClearOption::CO_Color | ClearOption::CO_Depth));


	MeshData* mesh = MeshData::create();
	mesh->mName = "Triangle";
	mesh->mVertices.emplace_back(-0.5f, -0.5f, 0.0f);	// left
	mesh->mVertices.emplace_back(0.5f, -0.5f, 0.0f);	// right
	mesh->mVertices.emplace_back(0.0f, 0.5f, 0.0f);	// top
	mesh->apply();

	mVertex = Vertex::create();
	mVertex->init(mesh);
	mVertex->generate();
	mVertex->addRef();

	auto shader = ShaderMgr::getInstance()->find("Tutorial/t01");
	auto queue = mObserver->getRenderQueue();
	this->addLogicFunction(this, [=]()
	{
		auto render_command = Graphics::getInstance()->createDrawVertexCMD(shader, mVertex);
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

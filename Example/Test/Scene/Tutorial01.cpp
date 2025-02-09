#include "Tutorial01.h"
#include "../MyObserver.h"

TINY_OBJECT_CPP(Tutorial01, Scene)

Tutorial01::Tutorial01(const std::string& name)
	: Base(name)
{

}

Tutorial01::~Tutorial01()
{
}

void Tutorial01::onEnter()
{
	Base::onEnter();

	LightingManager::disableEnvLighting();

	fmt::print("{0},{1},{2},{3}", this->getClassName(), this->getClassID(), this->getParentClassName(), Base::RTTIStatic()->__classID);

	MeshData* mesh = MeshData::create();
	mesh->mName = "Triangle";
	mesh->mVertices.emplace_back(-0.5f, -0.5f, 0.0f);	// left
	mesh->mVertices.emplace_back(0.5f, -0.5f, 0.0f);	// right
	mesh->mVertices.emplace_back(0.0f, 0.5f, 0.0f);		// top
	mesh->apply();

	mVertex = Vertex::create();
	mVertex->setMesh(mesh);
	mVertex->generate();
	mVertex->saveObject();

	auto observer = RenderObserver::create();
	observer->setOrderID(0);
	observer->setViewRect(0, 0, Engine::getScreenWidth(), Engine::getScreenHeight());
	observer->setClearOption(ClearOption(ClearOption::CO_Color | ClearOption::CO_Depth));

	auto shader = ShaderManager::find("Tutorial/t01");
	mPass = ReplacedPipelinePass::create(observer, shader);
	mPass->setPreFunction([=](ReplacedPipelinePass* pass)
	{
		pass->addCommand<RenderCMD_DrawVertex>(mVertex);
	});
	mPass->setFrameBuffer(FrameBufferManager::getMainFrameBufferBuildin());
	mPass->addToPipeline();
}

void Tutorial01::onExit()
{
	Base::onExit();
	mPass->removeFromPipeline();
	mVertex->deleteObject();
}

void Tutorial01::onPause()
{
	Base::onPause();
}

void Tutorial01::onResume()
{
	Base::onResume();
}

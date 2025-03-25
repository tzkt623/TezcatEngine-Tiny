#include "Tutorial_Triangle.h"
#include "../MyObserver.h"

TINY_OBJECT_CPP(Tutorial_Triangle, Scene);

Tutorial_Triangle::Tutorial_Triangle(const std::string& name)
	: Base(name)
{

}

Tutorial_Triangle::~Tutorial_Triangle()
{
}

void Tutorial_Triangle::onEnter()
{
	Base::onEnter();

	mVertex = Vertex::create();
	mVertex->setMesh(TutorialHelper::createTriangleMesh());
	mVertex->generate();
	mVertex->saveObject();

	mObserver = RenderObserver::create();
	mObserver->saveObject();
	mObserver->setOrderID(0);
	mObserver->setViewRect(0, 0, Engine::getScreenWidth(), Engine::getScreenHeight());
	mObserver->setClearOption(ClearOption(ClearOption::CO_Color | ClearOption::CO_Depth));

	auto shader = ShaderManager::find("Tutorial/Triangle");
	mPass = ReplacedPipelinePass::create(mObserver, shader);
	mPass->setCustomCulling([=](ReplacedPipelinePass* pass)
	{
		pass->addCommand<RenderCMD_Lambda>([=](PipelinePass* pass, Shader* shader)
			{
				Graphics::getInstance()->draw(mVertex);
			});
	});
	mPass->setFrameBuffer(FrameBufferManager::getMainFrameBufferBuildin());
	mPass->addToPipeline();
}

void Tutorial_Triangle::onExit()
{
	Base::onExit();
	mPass->removeFromPipeline();
	mVertex->deleteObject();
	mObserver->deleteObject();
}

void Tutorial_Triangle::onPause()
{
	Base::onPause();
}

void Tutorial_Triangle::onResume()
{
	Base::onResume();
}



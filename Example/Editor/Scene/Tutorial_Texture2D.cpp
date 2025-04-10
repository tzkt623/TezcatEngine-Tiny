#include "Tutorial_Texture2D.h"

TINY_OBJECT_CPP(Tutorial_Texture2D, Scene);
Tutorial_Texture2D::Tutorial_Texture2D(const std::string& name)
	: Base(name)
{

}

Tutorial_Texture2D::~Tutorial_Texture2D()
{

}

void Tutorial_Texture2D::onEnter()
{
	Base::onEnter();

	mVertex = Vertex::create();
	mVertex->setMesh(TutorialHelper::createRectMesh());
	mVertex->generate();
	mVertex->saveObject();

	mObserver = RenderObserver::create();
	mObserver->saveObject();
	mObserver->setSortingID(0);
	mObserver->setViewRect(0, 0, Engine::getScreenWidth(), Engine::getScreenHeight());
	mObserver->setClearOption(ClearOption(ClearOption::CO_Color | ClearOption::CO_Depth));

	auto mTexture = ResourceManager::loadAndSave<Texture2D>("Resource/Image/eldenring2.jpg");

	auto shader = ShaderManager::find("Tutorial/Texture2D");
	auto config = shader->getUniformValueConfig("myTexColor2D");
	mPass = ReplacedPipelinePass::create(mObserver, shader);
	mPass->setCustomCulling([=](ReplacedPipelinePass* pass)
	{
		pass->addCommand<RenderCMD_Lambda>([=](PipelinePass* pass, Shader* shader)
			{
				Graphics::getInstance()->bind(mTexture);
				Graphics::getInstance()->setTexture2D(shader, config->valueID, mTexture);
				Graphics::getInstance()->draw(mVertex);
			});
	});

	mPass->setFrameBuffer(FrameBufferManager::getMainFrameBufferBuildin());
	mPass->addToPipeline();
}

void Tutorial_Texture2D::onExit()
{
	Base::onExit();
	mPass->removeFromPipeline();
	mVertex->deleteObject();
	mObserver->deleteObject();
}

void Tutorial_Texture2D::onPause()
{
	Base::onPause();
}

void Tutorial_Texture2D::onResume()
{
	Base::onResume();
}


#include "Tutorial_Cube.h"

TINY_OBJECT_CPP(Tutorial_Cube, Scene);

Tutorial_Cube::Tutorial_Cube(const std::string& name)
	: Base(name)
{

}

Tutorial_Cube::~Tutorial_Cube()
{
}

void Tutorial_Cube::onEnter()
{
	Base::onEnter();

	mVertex = Vertex::create();
	mVertex->setMesh(TutorialHelper::createCubeMesh());
	mVertex->generate();
	mVertex->saveObject();

	mObserver = RenderObserver::create();
	mObserver->saveObject();
	mObserver->setSortingID(0);
	//mObserver->setPerspective(75.0f, 0.01f, 100.0f);
	mObserver->setViewRect(0, 0, EngineContext::ScreenWidth, EngineContext::ScreenHeight);
	mObserver->setClearOption({ ClearOption::CO_Color | ClearOption::CO_Depth });

	auto shader = ShaderManager::find("Tutorial/Cube");
	auto config_mvp = shader->getUserUniformValueConfig("MVP");
	auto config_color = shader->getUserUniformValueConfig("myColor");

	mPass = ReplacedPipelinePass::create(mObserver, shader);
	mPass->setCustomCulling([=](ReplacedPipelinePass* pass)
	{
		pass->addCommand<RenderCMD_Lambda>([=](PipelinePass* pass, Shader* shader)
			{
				auto projection = glm::perspective(glm::radians(75.0f), (float)EngineContext::ScreenWidth / EngineContext::ScreenHeight, 0.01f, 100.0f);
				auto view = glm::lookAt(float3(0.0f, 0.0f, 2.0f)
					, float3(0.0f, 0.0f, -1.0f)
					, float3(0.0f, 1.0f, 0.0f));
				auto model = glm::translate(float4x4(1.0f), float3(0.0f, 0.0f, -1.0f))
					* glm::rotate(float4x4(1.0f), 45.0f, float3(0.0f, 1.0f, 0.0f));
				auto mvp = projection * view * model;
				Graphics::getInstance()->setMat4(shader, config_mvp->valueID, mvp);
				Graphics::getInstance()->setFloat3(shader, config_color->valueID, float3(1.0f, 0.5f, 0.5f));
				Graphics::getInstance()->draw(mVertex);
			});
	});
	mPass->setFrameBuffer(FrameBufferManager::getMainFrameBufferBuildin());
	mPass->addToPipeline();
}

void Tutorial_Cube::onExit()
{
	Base::onExit();
	mPass->removeFromPipeline();
	mVertex->deleteObject();
	mObserver->deleteObject();
}

void Tutorial_Cube::onPause()
{
	Base::onPause();
}

void Tutorial_Cube::onResume()
{
	Base::onResume();
}


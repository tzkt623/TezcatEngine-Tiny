#include "Tutorial_Camera.h"

TINY_OBJECT_CPP(Tutorial_Camera, Scene);

Tutorial_Camera::Tutorial_Camera(const std::string& name)
	: Base(name)
{

}

Tutorial_Camera::~Tutorial_Camera()
{
}

void Tutorial_Camera::onEnter()
{
	Base::onEnter();

	mVertex = Vertex::create();
	mVertex->setMesh(TutorialHelper::createCubeMesh());
	mVertex->generate();
	mVertex->saveObject();

	mObserver = RenderObserver::create();
	mObserver->saveObject();
	mObserver->setOrderID(0);
	mObserver->setViewRect(0, 0, Engine::getScreenWidth(), Engine::getScreenHeight());
	mObserver->setClearOption({ ClearOption::CO_Color | ClearOption::CO_Depth });

	auto shader = ShaderManager::find("Tutorial/Cube");
	auto config_mvp = shader->getUniformValueConfig("MVP");
	auto config_color = shader->getUniformValueConfig("myColor");

	mProjectionMatrix = glm::perspective(glm::radians(75.0f), (float)Engine::getScreenWidth() / Engine::getScreenHeight(), 0.01f, 100.0f);
	mViewMatrix = glm::lookAt(float3(0.0f, 0.0f, 0.0f)
		, float3(0.0f, 0.0f, -1.0f)
		, float3(0.0f, 1.0f, 0.0f));

	mPass = ReplacedPipelinePass::create(mObserver, shader);
	mPass->setCustomCulling([=, this](ReplacedPipelinePass* pass)
	{
		mViewMatrix = glm::rotate(mViewMatrix, glm::radians(18.0f) * Engine::getDeltaTime(), float3(0.0f, 1.0f, 0.0f));

		{//cube
			pass->addCommand<RenderCMD_Lambda>([=, this](PipelinePass* pass, Shader* shader)
			{
				auto model = glm::translate(float4x4(1.0f), float3(-3.0f, 0.0f, 0.0f))
					* glm::rotate(float4x4(1.0f), 45.0f, float3(0.0f, 1.0f, 0.0f));
				auto mvp = mProjectionMatrix * mViewMatrix * model;
				Graphics::getInstance()->setMat4(shader, config_mvp->valueID, mvp);
				Graphics::getInstance()->setFloat3(shader, config_color->valueID, float3(1.0f, 0.0f, 0.0f));
				Graphics::getInstance()->draw(mVertex);
			});

			pass->addCommand<RenderCMD_Lambda>([=, this](PipelinePass* pass, Shader* shader)
			{
				auto model = glm::translate(float4x4(1.0f), float3(0.0f, 0.0f, -3.0f))
					* glm::rotate(float4x4(1.0f), 45.0f, float3(0.0f, 1.0f, 0.0f));
				auto mvp = mProjectionMatrix * mViewMatrix * model;
				Graphics::getInstance()->setMat4(shader, config_mvp->valueID, mvp);
				Graphics::getInstance()->setFloat3(shader, config_color->valueID, float3(0.0f, 1.0f, 0.0f));
				Graphics::getInstance()->draw(mVertex);
			});

			pass->addCommand<RenderCMD_Lambda>([=, this](PipelinePass* pass, Shader* shader)
			{
				auto model = glm::translate(float4x4(1.0f), float3(3.0f, 0.0f, 0.0f))
					* glm::rotate(float4x4(1.0f), 45.0f, float3(0.0f, 1.0f, 0.0f));
				auto mvp = mProjectionMatrix * mViewMatrix * model;
				Graphics::getInstance()->setMat4(shader, config_mvp->valueID, mvp);
				Graphics::getInstance()->setFloat3(shader, config_color->valueID, float3(0.0f, 0.0f, 1.0f));
				Graphics::getInstance()->draw(mVertex);
			});

			pass->addCommand<RenderCMD_Lambda>([=, this](PipelinePass* pass, Shader* shader)
			{
				auto model = glm::translate(float4x4(1.0f), float3(0.0f, 0.0f, 3.0f))
					* glm::rotate(float4x4(1.0f), 45.0f, float3(0.0f, 1.0f, 0.0f));
				auto mvp = mProjectionMatrix * mViewMatrix * model;
				Graphics::getInstance()->setMat4(shader, config_mvp->valueID, mvp);
				Graphics::getInstance()->setFloat3(shader, config_color->valueID, float3(1.0f, 0.5f, 0.5f));
				Graphics::getInstance()->draw(mVertex);
			});
		}
	});
	mPass->setFrameBuffer(FrameBufferManager::getMainFrameBufferBuildin());
	mPass->addToPipeline();
}

void Tutorial_Camera::onExit()
{
	Base::onExit();
	mPass->removeFromPipeline();
	mVertex->deleteObject();
	mObserver->deleteObject();
}

void Tutorial_Camera::onPause()
{
	Base::onPause();
}

void Tutorial_Camera::onResume()
{
	Base::onResume();
}


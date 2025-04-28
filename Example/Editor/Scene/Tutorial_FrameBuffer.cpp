#include "Tutorial_FrameBuffer.h"

TINY_OBJECT_CPP(Tutorial_FrameBuffer, Scene);

Tutorial_FrameBuffer::Tutorial_FrameBuffer(const std::string& name)
	: Base(name)
	, mGenerator(0.0f, 1.0f)
	, mColor(0.0f)
{

}

Tutorial_FrameBuffer::~Tutorial_FrameBuffer()
{
}

void Tutorial_FrameBuffer::onEnter()
{
	Base::onEnter();

	mVertex = Vertex::create();
	mVertex->setMesh(TutorialHelper::createCubeMesh());
	mVertex->generate();
	mVertex->saveObject();

	mObserver = RenderObserver::create();
	mObserver->saveObject();
	mObserver->setSortingID(-127);
	mObserver->setViewRect(0, 0, EngineContext::ScreenWidth, EngineContext::ScreenHeight);
	mObserver->setClearOption({ ClearOption::CO_Color | ClearOption::CO_Depth });

	auto shader = ShaderManager::find("Tutorial/Cube");
	auto config_mvp = shader->getUserUniformValueConfig("MVP");
	auto config_color = shader->getUserUniformValueConfig("myColor");

	mTimeBegin = std::chrono::high_resolution_clock::now();

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

				auto color = this->randomColor();

				Graphics::getInstance()->setFloat3(shader, config_color->valueID, color);
				Graphics::getInstance()->draw(mVertex);
			});
	});

	auto [flag, frame_buffer] = FrameBufferManager::create("Tutorial_FrameBuffer");
	if (flag == FlagCreate::Succeeded)
	{
		auto [flag2, tex_color] = TextureManager::create2D("Tutorial_FrameBuffer_TexColor");
		if (flag2 == FlagCreate::Succeeded)
		{
			tex_color->setConfig(EngineContext::ScreenWidth, EngineContext::ScreenHeight
				, TextureInternalFormat::RGBA
				, TextureFormat::RGBA);
			tex_color->setAttachPosition(TextureAttachPosition::ColorComponent);
			frame_buffer->addAttachment(tex_color);
		}

		auto [flag3, tex_depth] = TextureManager::create2D("Tutorial_FrameBuffer_TexDepth");
		if (flag3 == FlagCreate::Succeeded)
		{
			tex_depth->setConfig(EngineContext::ScreenWidth, EngineContext::ScreenHeight
				, TextureInternalFormat::Depth
				, TextureFormat::Depth);
			tex_depth->setAttachPosition(TextureAttachPosition::DepthComponent);
			frame_buffer->addAttachment(tex_depth);
		}

		frame_buffer->generate();
		mFrameBuffer = frame_buffer;
	}
	
	mPass->setFrameBuffer(mFrameBuffer);
	mPass->addToPipeline();
}

void Tutorial_FrameBuffer::onExit()
{
	Base::onExit();
	mPass->removeFromPipeline();
	mVertex->deleteObject();
	mObserver->deleteObject();

	TextureManager::remove("Tutorial_FrameBuffer_TexColor");
	TextureManager::remove("Tutorial_FrameBuffer_TexDepth");
	FrameBufferManager::remove("Tutorial_FrameBuffer");
}

void Tutorial_FrameBuffer::onPause()
{
	Base::onPause();
}

void Tutorial_FrameBuffer::onResume()
{
	Base::onResume();
}

float3 Tutorial_FrameBuffer::randomColor()
{
	auto now = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float, std::ratio<1>> delay = now - mTimeBegin;

	if (delay.count() > 1.0f)
	{
		mTimeBegin = now;
		mColor = float3(mGenerator(mRandom), mGenerator(mRandom), mGenerator(mRandom));
	}

	return mColor;
}



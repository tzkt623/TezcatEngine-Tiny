#include "EditorEngineIniter.h"
#include "EditorInputer.h"
#include "EditorEvent.h"

#include "Scene/SceneHead.h"


using namespace tezcat::Tiny::GL;
#define CreateWindow(host, X) (new X())->open(host)
namespace tezcat::Editor
{
	EditorEngineIniter::EditorEngineIniter()
		: EngineIniter(RenderAPI::OpenGL)
	{

	}

	void EditorEngineIniter::prepareEngine(Engine* engine)
	{
		TINY_LOG_ENGINE("Prepare Engine.....");
		EngineIniter::prepareEngine(engine);
		EditorEvent::get()->init(EditorEventID::Count);

		//engine->setEnableMultiThread();

		//mResourceFolderName = "Resource";
		//mGameName = u8"V0.3(没有黑夜君临玩我要死了)";
		EngineContext::Name = u8"V0.3(没有黑夜君临玩我要死了)";
		EngineContext::ScreenWidth = 1920;
		EngineContext::ScreenHeight = 1080;
		EngineContext::EnableVsync = true;

		this->setGLVersion(4, 3);
	}

	void EditorEngineIniter::prepareResource(Engine* engine)
	{
		TINY_LOG_ENGINE("Prepare Resource.....");
		EngineIniter::prepareResource(engine);
		ShaderManager::loadShaderFiles("Resource/Shaders/Tutorial");
	}

	void EditorEngineIniter::prepareGame(Engine* engine)
	{
		TINY_LOG_ENGINE("Prepare Scene.....");
		EngineIniter::prepareGame(engine);

		MyMainScene::create("MainScene")->prepare();
		SceneHelper::prepareTutorialScene();
	}

	void EditorEngineIniter::initYourShaderParam()
	{
		//std::cout << "Init My Shader Params\n";
	}
}

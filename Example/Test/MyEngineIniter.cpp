#include "MyEngineIniter.h"
#include "MyInputer.h"
#include "MyGUI/MyGUI.h"
#include "MyEvent.h"

#include "Scene/SceneHead.h"


using namespace tezcat::Tiny::GL;
#define CreateWindow(host, X) (new X())->open(host)
namespace tezcat::Editor
{
	MyEngineIniter::MyEngineIniter()
		: EngineIniter(RenderAPI::OpenGL)
	{

	}

	void MyEngineIniter::prepareEngine(Engine* engine)
	{
		EngineIniter::prepareEngine(engine);
		MyEvent::get()->init(MyEventID::Count);

		//engine->setEnableMultiThread();

		mResourceFolderName = "Resource";
		mGameName = u8"V0.3(没有黑夜君临玩我要死了)";
		mWindowWidth = 1920;
		mWindowHeight = 1080;
		mEnableVsync = true;
		this->setGLVersion(4, 3);
	}

	void MyEngineIniter::prepareResource(Engine* engine)
	{
		EngineIniter::prepareResource(engine);

		//TextureManager::loadResource("/Image");
		//ModelMgr::getInstance()->loadModel("./Resource/Model/Tree04.fbx");
	}

	void MyEngineIniter::prepareGame(Engine* engine)
	{
		EngineIniter::prepareGame(engine);
		ShaderManager::loadShaderFiles(FileTool::getRootRelativeResDir() + "/Shaders/Tutorial");

		MyMainScene::create("MainScene")->prepare();
		SceneHelper::prepareTutorialScene();
	}

	void MyEngineIniter::initYourShaderParam()
	{
		std::cout << "Init My Shader Params\n";
	}
}

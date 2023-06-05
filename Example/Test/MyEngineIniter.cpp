#include "MyEngineIniter.h"
#include "MyInputer.h"
#include "MyGUI/MyGUI.h"
#include "MyEvent.h"

#include "Scene/MyMainScene.h"
#include "Scene/MySecondScene.h"
#include "Scene/Tutorial01.h"


using namespace tezcat::Tiny::GL;
#define CreateWindow(host, X) (new X())->open(host)

MyEngineIniter::MyEngineIniter()
	: EngineIniter(RenderAPI::OpenGL)
{

}

void MyEngineIniter::prepareEngine(Engine* engine)
{
	EngineIniter::prepareEngine(engine);
	MyEvent::get()->init(MyEventID::Count);

	engine->setEnableMultiThread();

	mResourceFolderName = "Resource";
	mGameName = u8"YesIndeed,玩上老头环了!!!!!";
	mWindowWidth = 1920;
	mWindowHeight = 1080;
	mEnableVsync = true;
	this->setGLVersion(3, 3);
}

void MyEngineIniter::prepareResource(Engine* engine)
{
	EngineIniter::prepareResource(engine);

	TextureMgr::getInstance()->loadResource("/Image");

	//ModelMgr::getInstance()->loadModel("./Resource/Model/Tree04.fbx");
}

void MyEngineIniter::prepareGame(Engine* engine)
{
	EngineIniter::prepareGame(engine);
	ShaderMgr::getInstance()->loadShaderFiles(FileTool::getRootRelativeResDir() + "/Shaders/Tutorial");

	auto main_window = new MyMainWindow();
	main_window->open(Graphics::getInstance()->mGUI);
	main_window->init();

	SceneMgr::getInstance()->prepareScene(MyMainScene::create("MainScene"));
	//SceneMgr::getInstance()->prepareScene(MySeconedScene::create("SecondScene"));
	SceneMgr::getInstance()->prepareScene(Tutorial01::create("Tutorial01"));
}

void MyEngineIniter::initYourShaderParam()
{
	std::cout << "Init My Shader Params\n";
}

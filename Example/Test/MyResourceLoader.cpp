#include "MyResourceLoader.h"
#include "MyInputer.h"
#include "MyGUI/MyGUI.h"
#include "MyEvent.h"

#include "Scene/MyMainScene.h"
#include "Scene/MySecondScene.h"
#include "Scene/Tutorial01.h"


using namespace tezcat::Tiny::GL;
#define CreateWindow(host, X) (new X())->open(host)

MyResourceLoader::MyResourceLoader()
	: ResourceLoader(RenderAPI::OpenGL)
{

}

void MyResourceLoader::prepareEngine(Engine* engine)
{
	ResourceLoader::prepareEngine(engine);
	MyEvent::get()->init(MyEventID::Count);

	static_cast<WindowsEditor*>(engine)->setGLVersion(3, 3);

	mResourceFolderName = "Resource";
	mGameName = u8"YesIndeed,玩上老头环了!!!!!";
	mWindowWidth = 1920;
	mWindowHeight = 1080;
	mEnableVsync = true;
}

void MyResourceLoader::prepareResource(Engine* engine)
{
	ResourceLoader::prepareResource(engine);
	TextureMgr::getInstance()->loadResource("/Image");

	//ModelMgr::getInstance()->loadModel("./Resource/Model/Tree04.fbx");
}

void MyResourceLoader::prepareGame(Engine* engine)
{
	ResourceLoader::prepareGame(engine);
	ShaderMgr::getInstance()->loadShaderFiles(FileTool::getRootRelativeResDir() + "/Shaders/Tutorial");

	auto gui_host = static_cast<WindowsEditor*>(engine)->getGUI();
	auto main_window = new MyMainWindow();
	main_window->open(gui_host);
	main_window->init();

	SceneMgr::getInstance()->prepareScene(MyMainScene::create("MainScene"));
	//SceneMgr::getInstance()->prepareScene(MySeconedScene::create("SecondScene"));
	SceneMgr::getInstance()->prepareScene(Tutorial01::create("Tutorial01"));
}

void MyResourceLoader::initYourShaderParam()
{
	std::cout << "Init My Shader Params\n";
}

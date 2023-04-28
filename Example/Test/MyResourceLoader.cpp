#include "MyResourceLoader.h"
#include "MyInputer.h"
#include "MyGUI/MyGUI.h"
#include "MyEvent.h"

#include "Scene/MyMainScene.h"
#include "Scene/MySecondScene.h"



using namespace tezcat::Tiny::GL;
#define CreateWindow(host, X) (new X)->open(host)
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
}

void MyResourceLoader::prepareGame(Engine* engine)
{
	ResourceLoader::prepareGame(engine);
	auto gui_host = static_cast<WindowsEditor*>(engine)->getGUI();

	CreateWindow(gui_host, MyMainDockWindow);
	CreateWindow(gui_host, MyViewPortWindow);
	CreateWindow(gui_host, MyObjectWindow);
	CreateWindow(gui_host, MyOverviewWindow);
	CreateWindow(gui_host, MyLogWindow);
	CreateWindow(gui_host, MyGCInfoWindow);

	engine->getGraphics()->setShadowMap(1024, 1024);

 	SceneMgr::getInstance()->prepareScene(MyMainScene::create("MainScene"));
 	SceneMgr::getInstance()->prepareScene(MySeconedScene::create("SecondScene"));
 	//SceneMgr::getInstance()->pushScene("MyScene");
}

void MyResourceLoader::initYourShaderParam()
{
	std::cout << "Init My Shader Params\n";
}

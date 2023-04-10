#include "MyResourceLoader.h"
#include "MyScene.h"
#include "MyInputer.h"

MyResourceLoader::MyResourceLoader()
	: ResourceLoader(RenderAPI::OpenGL)
{

}

void MyResourceLoader::prepareEngine(Engine* engine)
{
	ResourceLoader::prepareEngine(engine);
	engine->getGraphics()->setShadowMap(4096, 4096);
	mResourceFolderName = "Resource";
	mGameName = u8"YesIndeed,玩上老头环了!!!!!";
	mWindowWidth = 1920;
	mWindowHeight = 1080;

}

void MyResourceLoader::prepareResource(Engine* engine)
{
	ResourceLoader::prepareResource(engine);
	TextureMgr::getInstance()->loadResource("/Image");
}

void MyResourceLoader::prepareGame(Engine* engine)
{
	ResourceLoader::prepareGame(engine);
	SceneMgr::getInstance()->prepareScene(new MyScene("MyScene"));
	SceneMgr::getInstance()->pushScene("MyScene");

	//	Graphics::getInstance()->createShadowRenderer("ShadowMap", 1024, 1024);
}

void MyResourceLoader::initYourShaderParam()
{
	std::cout << "Init My Shader Params\n";
}

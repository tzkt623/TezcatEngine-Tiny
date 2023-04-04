#include "MyResourceLoader.h"
#include "MyScene.h"
#include "MyInputer.h"

MyResourceLoader::MyResourceLoader()
	: ResourceLoader(RenderAPI::OpenGL)
{

}

void MyResourceLoader::prepareEngine(Engine* engine)
{
	mGameName = u8"YesIndeed,玩上老头环了!!!!!";
	mWindowWidth = 1920;
	mWindowHeight = 1080;
}



void MyResourceLoader::prepareScene(Engine* engine)
{
	SG<SceneManager>::getInstance()->prepareScene(new MyScene("MyScene"));
	SG<SceneManager>::getInstance()->pushScene("MyScene");
}

void MyResourceLoader::prepareResource(Engine* engine)
{
	ResourceLoader::prepareResource(engine);
}

void MyResourceLoader::initYourShaderParam()
{
	std::cout << "Init My Shader Params\n";
}

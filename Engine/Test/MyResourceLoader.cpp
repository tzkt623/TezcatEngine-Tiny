#include "MyResourceLoader.h"
#include "MyScene.h"

void MyResourceLoader::prepareEngine(Core::Engine* engine)
{
	m_GameName = u8"ÎÒµÄÓÎÏ·";
	m_WindowWidth = 1600;
	m_WindowHeight = 900;
}

void MyResourceLoader::prepareScene(Engine* engine)
{
	engine->getSceneManager()->prepareScene(new MyScene("MyScene"));
	engine->getSceneManager()->pushScene("MyScene");
}

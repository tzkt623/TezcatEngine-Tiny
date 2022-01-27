#include "MyResourceLoader.h"
#include "MyScene.h"

void MyResourceLoader::prepareScene(Engine* engine)
{
	engine->getSceneManager()->prepareScene(new MyScene("MyScene"));
	engine->getSceneManager()->pushScene("MyScene");
}

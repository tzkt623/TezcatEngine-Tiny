#include "MyObserver.h"

MyObserver::MyObserver()
	: IRenderObserver(new ExtraQueue(this))
{

}

MyObserver::~MyObserver()
{

}


void MyObserver::submit(BaseGraphics* graphics, Shader* shader)
{

}

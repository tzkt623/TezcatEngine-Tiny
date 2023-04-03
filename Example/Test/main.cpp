#include "MyResourceLoader.h"

int main()
{
	auto engine = new GL::WindowsEngine();
	if (engine->init(new MyResourceLoader()))
	{
		engine->run();
	}
	return 0;
}

#include "Tiny.h"
#include "MyResourceLoader.h"

int main()
{
	auto engine = new WindowsEngine();
	if (engine->init(new MyResourceLoader()))
	{
		engine->run();
	}
	return 0;
}
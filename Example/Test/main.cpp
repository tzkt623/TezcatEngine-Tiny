#include "MyResourceLoader.h"

using namespace tezcat::Tiny::GL;

int main()
{
	auto engine = new WindowsEngine();
	if (engine->init(new MyResourceLoader()))
	{
		engine->run();
	}
	return 0;
}

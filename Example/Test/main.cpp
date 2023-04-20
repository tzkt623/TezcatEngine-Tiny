#include "MyResourceLoader.h"
//#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
using namespace tezcat::Tiny::GL;

int main()
{
	auto engine = new WindowsEditor();
	if (engine->init(new MyResourceLoader()))
	{
		engine->run();
	}
	return 0;
}

#include "MyEngineIniter.h"
#include "WindowsEngine.h"

//#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

using namespace tezcat::Editor;

int main()
{
	auto engine = new WindowsEditor();
	if (engine->init(new MyEngineIniter()))
	{
		engine->run();
	}
	return 0;
}


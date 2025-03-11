#include "MyEngineIniter.h"
#include "WindowsEngine.h"

//#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

using namespace tezcat::Editor;

int main()
{
	std::locale locale("zh_CN.UTF-8");
	std::locale::global(locale);
	//std::cout.imbue(locale);

	auto engine = new WindowsEditor();
	if (engine->init(new MyEngineIniter()))
	{
		engine->run();
	}
	return 0;
}


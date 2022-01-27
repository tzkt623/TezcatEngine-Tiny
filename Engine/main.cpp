#include "Tiny.h"
#include "MyResourceLoader.h"

using namespace tezcat::Tiny;

int main()
{
	auto result = Core::Engine::getInstance()
		->init(u8"如果标题不取der非常非常非常der长der话就没法以引起大家的注意了.那这么长的标题还有什么用?你说是吧!啊啊啊?", 1600, 900);

	if (result == 0)
	{
		Core::Engine::getInstance()->setOn(new MyResourceLoader());
		Core::Engine::getInstance()->run();
	}

	return result;
}

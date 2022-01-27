#include "Tiny.h"
#include "MyResourceLoader.h"

using namespace tezcat::Tiny;

int main()
{
	auto result = Core::Engine::getInstance()
		->init(u8"������ⲻȡder�ǳ��ǳ��ǳ�der��der����û���������ҵ�ע����.����ô���ı��⻹��ʲô��?��˵�ǰ�!������?", 1600, 900);

	if (result == 0)
	{
		Core::Engine::getInstance()->setOn(new MyResourceLoader());
		Core::Engine::getInstance()->run();
	}

	return result;
}

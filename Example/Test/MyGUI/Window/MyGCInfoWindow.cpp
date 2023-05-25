#include "MyGCInfoWindow.h"


CreateInstanceCPP(MyGCInfoWindow);
MyGCInfoWindow::MyGCInfoWindow()
	: GUIWindow("GCInfo")
{

}

MyGCInfoWindow::~MyGCInfoWindow()
{
	DeleteInstance();
}

void MyGCInfoWindow::onRender()
{
	GUIWindow::onRender();

	auto& infos = TinyGC::getGCInfos();

	for (auto i : infos)
	{
		if (i->strongRef > 0)
		{
			ImGui::Text("[%d]: <%d, %d> %s"
				, i->index
				, i->strongRef
				, i->weakRef
				, i->pointer->getClassName().c_str());
		}
		else
		{
			ImGui::Text("[%d]: <%d, %d> %s"
				, i->index
				, i->strongRef
				, i->weakRef
				, "FreeSlot");
		}
	}
}

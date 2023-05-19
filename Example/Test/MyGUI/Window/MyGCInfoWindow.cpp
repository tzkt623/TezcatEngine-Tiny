#include "MyGCInfoWindow.h"



MyGCInfoWindow::MyGCInfoWindow()
	: GUIWindow("GCInfo")
{

}

MyGCInfoWindow::~MyGCInfoWindow()
{

}

void MyGCInfoWindow::onRender()
{
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

void MyGCInfoWindow::onUpdate()
{
}

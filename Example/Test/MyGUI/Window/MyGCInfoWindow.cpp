#include "MyGCInfoWindow.h"

namespace tezcat::Editor
{
	TINY_EDITOR_WINDOW_INSTANCE_CPP(MyGCInfoWindow)
	MyGCInfoWindow::MyGCInfoWindow()
		: GUIWindow("GCInfo")
	{

	}

	MyGCInfoWindow::~MyGCInfoWindow()
	{
		TINY_EDITOR_WINDOW_DELETE_INSTACNE();
	}

	void MyGCInfoWindow::onRender()
	{
		GUIWindow::onRender();

		auto& infos = TinyGC::getGCInfos();
		ImGuiListClipper clipper;
		clipper.Begin(infos.size(), ImGui::GetTextLineHeightWithSpacing());
		while (clipper.Step())
		{
			for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
			{
				auto i = infos[line_no];
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
		clipper.End();
	}
}

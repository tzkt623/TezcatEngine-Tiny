/*
	Copyright (C) 2025 Tezcat(特兹卡特) tzkt623@qq.com

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "EditorGCInfoWindow.h"

namespace tezcat::Editor
{
	TINY_EDITOR_WINDOW_INSTANCE_CPP(EditorGCInfoWindow)
	EditorGCInfoWindow::EditorGCInfoWindow()
		: GUIWindow("GCInfo")
	{

	}

	EditorGCInfoWindow::~EditorGCInfoWindow()
	{
		TINY_EDITOR_WINDOW_DELETE_INSTACNE();
	}

	void EditorGCInfoWindow::onRender()
	{
		GUIWindow::onRender();

		auto& infos = TinyGC::getGCInfos();

		ImGui::BeginTable("##GCInfo", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_SizingFixedFit);

		ImGuiListClipper clipper;
		clipper.Begin(infos.size(), ImGui::GetTextLineHeightWithSpacing());
		while (clipper.Step())
		{
			for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
			{
				ImGui::TableNextRow();
				auto& i = infos[line_no];
				if (i->strongRef > 0)
				{
					ImGui::TableNextColumn();
					ImGui::Text("[%d]: <%d, %d> %s"
						, i->index
						, i->strongRef
						, i->weakRef
						, i->pointer->getClassName().c_str());
					ImGui::TableNextColumn();
					ImGui::Text("%s"
						, i->pointer->getMemoryInfo().c_str());
				}
				else
				{
					ImGui::TableNextColumn();
					ImGui::Text("[%d]: <%d, %d> %s"
						, i->index
						, i->strongRef
						, i->weakRef
						, "FreeSlot");
					ImGui::TableNextColumn();
					ImGui::Text("-----");
				}
			}
		}
		clipper.End();

		ImGui::EndTable();
	}
}

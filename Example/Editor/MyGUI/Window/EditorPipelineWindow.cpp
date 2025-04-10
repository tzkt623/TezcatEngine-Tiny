#include "EditorPipelineWindow.h"

namespace tezcat::Editor
{
	TINY_EDITOR_WINDOW_INSTANCE_CPP(EditorPipelineWindow);
	EditorPipelineWindow::EditorPipelineWindow()
		: GUIWindow("管线查看器(PipelineViewer)")
	{

	}

	EditorPipelineWindow::~EditorPipelineWindow()
	{
		TINY_EDITOR_WINDOW_DELETE_INSTACNE();
	}

	void EditorPipelineWindow::onRender()
	{
		Profiler::EnableRenderInfo = true;

		auto cursor_pos = ImGui::GetCursorPosX();
		auto space = ImGui::GetStyle().IndentSpacing;

		ImGuiListClipper clipper;
		clipper.Begin(Profiler::RenderInfoList.size(), ImGui::GetTextLineHeightWithSpacing());
		while (clipper.Step())
		{
			for (auto i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
			{
				ImGui::SetCursorPosX(cursor_pos + space * Profiler::RenderInfoList[i].layer);
				ImGui::Text(Profiler::RenderInfoList[i].info.c_str());
				ImGui::SetCursorPosX(cursor_pos);
			}
		}
		clipper.End();

		Profiler::pipelineInfoClear();
	}

	void EditorPipelineWindow::open(GUI* gui)
	{
		GUIWindow::open(gui);
		Profiler::EnableRenderInfo = true;
	}

	void EditorPipelineWindow::close()
	{
		GUIWindow::close();
		Profiler::EnableRenderInfo = false;
		Profiler::pipelineInfoClear();
	}

	void EditorPipelineWindow::onNotRender()
	{
		Profiler::EnableRenderInfo = false;
		Profiler::pipelineInfoClear();
	}
}


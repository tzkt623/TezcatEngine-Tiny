#include "EditorInfoWindow.h"


namespace tezcat::Editor
{
	EditorInfoWindow::EditorInfoWindow()
		: GUIWindow(u8"信息(Info)")
		, mFrameTimeBuffer(40, 0)

	{

	}

	EditorInfoWindow::~EditorInfoWindow()
	{

	}

	void EditorInfoWindow::init()
	{

	}

	void EditorInfoWindow::onRender()
	{
		GUIWindow::onRender();

		//gpu
		ImGui::Text("GPU: %s", Profiler::GPU);
		//memory
		ImGui::Text("Memory: %.3f kb", Profiler::getMemoryUse() / 1024.0f);
		//logic time
		ImGui::Text("LogicTime: %.1f ms", Profiler::LogicTime);

		//render time
		mFrameTimeBuffer.push_back((float)Profiler::RenderTime);
		if (mFrameTimeBuffer.size() > 40)
		{
			mFrameTimeBuffer.erase(mFrameTimeBuffer.begin());
		}
		ImGui::SetNextItemWidth(200);
		ImGui::PlotLines("Frame Times", &mFrameTimeBuffer[0], (int)mFrameTimeBuffer.size());
		ImGui::SameLine();
		ImGui::Text("%.1f ms", Profiler::RenderTime);

		//fps
		ImGui::Text("FPS: %.1f(%.3f ms/Frame)", GUIFunction::getFrameRate(), GUIFunction::getFrameRateTime());

		//draw
		ImGui::Text("PassCount: %d", Profiler::PassCount);
		ImGui::Text("DrawCount: %d", Profiler::DrawCall);

		//mouse
		ImGui::InputFloat2("MousePosition", glm::value_ptr(Profiler::mousePosition));
		ImGui::InputFloat2("MouseOffset", glm::value_ptr(Profiler::mouseOffset));
	}
}

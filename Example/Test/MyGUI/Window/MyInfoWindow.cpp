#include "MyInfoWindow.h"



MyInfoWindow::MyInfoWindow()
	: GUIWindow(u8"信息(Info)")
	, mFrameTimeBuffer(40, 0)

{

}

MyInfoWindow::~MyInfoWindow()
{

}

void MyInfoWindow::init()
{

}

void MyInfoWindow::onUpdate()
{
	//gpu
	ImGui::Text("GPU: %s", Statistic::GPU);
	//memory
	ImGui::Text("Memory: %.3f kb", Statistic::getMemoryUse() / 1024.0f);
	//logic time
	ImGui::Text("LogicTime: %.1f ms", Statistic::LogicTime);

	//render time
	mFrameTimeBuffer.push_back((float)Statistic::RenderTime);
	if (mFrameTimeBuffer.size() > 40)
	{
		mFrameTimeBuffer.erase(mFrameTimeBuffer.begin());
	}
	ImGui::SetNextItemWidth(200);
	ImGui::PlotLines("Frame Times", &mFrameTimeBuffer[0], (int)mFrameTimeBuffer.size());
	ImGui::SameLine();
	ImGui::Text("%.1f ms", Statistic::RenderTime);

	//fps
	ImGui::Text("FPS: %.1f(%.3f ms/Frame)", GUIFunction::getFrameRate(), GUIFunction::getFrameRateTime());

	//draw
	ImGui::Text("PassCount: %d", Statistic::PassCount);
	ImGui::Text("DrawCount: %d", Statistic::DrawCall);

	//mouse
	ImGui::InputFloat2("MousePosition", glm::value_ptr(Statistic::mousePosition));
	ImGui::InputFloat2("MouseOffset", glm::value_ptr(Statistic::mouseOffset));
}

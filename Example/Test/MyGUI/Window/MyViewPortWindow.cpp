#include "MyViewPortWindow.h"




MyViewPortWindow::MyViewPortWindow()
	: GUIWindow("Viewport")
	, mColorBuffer(nullptr)
{

}

MyViewPortWindow::~MyViewPortWindow()
{

}

void MyViewPortWindow::begin()
{
	ImGui::Begin(this->getName(), 0, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_MenuBar);
}

void MyViewPortWindow::onUpdate()
{

}

void MyViewPortWindow::calculate(const ImVec2& inTextureSize, const ImVec2& inWindowSize
	, ImVec2& outDisplaySize, ImVec2& outOffset, ImVec2& outUV0, ImVec2& outUV1)
{
	outDisplaySize = inWindowSize;

	//按照视图比例 得到符合当前视图的诊室窗口大小
	float ratio_x = inTextureSize.x / inTextureSize.y;
	float ratio_y = inTextureSize.y / inTextureSize.x;

	//先保证上下对齐,也就是需要计算左右偏移
	//获得当前比例下的宽度
	float width = outDisplaySize.y * ratio_x;
	//如果宽度比当前的小,需要把图片移动到view中心
	if (width < outDisplaySize.x)
	{
		//计算多出来的宽度的一半
		outOffset.x = (outDisplaySize.x - width) * 0.5f;
		outDisplaySize.x = outDisplaySize.y * ratio_x;
	}
	else
	{
		//宽度比当前的大
		//左右压缩满了
		//计算上下之间的空隙
		float height = outDisplaySize.x * ratio_y;
		outOffset.y = (outDisplaySize.y - height) * 0.5f;
		outDisplaySize.y = outDisplaySize.x * ratio_y;
	}

	//计算当前显示大小于视图的比值
	//
	outUV0 = ImVec2(0, outDisplaySize.y / inTextureSize.y);
	outUV1 = ImVec2(outDisplaySize.x / inTextureSize.x, 0);

	if (outUV0.y > 1)
	{
		outUV0.y = 1;
	}

	if (outUV1.x > 1)
	{
		outUV1.x = 1;
	}
}

void MyViewPortWindow::onRender()
{
	static bool bInfo;
	if (ImGui::BeginMenuBar())
	{
		ImGui::MenuItem("Info", 0, &bInfo);
		if (bInfo)
		{
			auto pos = ImGui::GetItemRectMin();
			pos.y += ImGui::GetItemRectSize().y;
			this->drawInfo(pos);
		}

		ImGui::EndMenuBar();
	}

// 	auto pos = ImGui::GetItemRectSize();
// 	Log::info(StringTool::stringFormat("MenuBarSize[%.1f,%.1f]", pos.x, pos.y));

	auto camera_data = CameraMgr::getInstance()->getCameraData();
	if (!camera_data.lock())
	{
		return;
	}

	if (!camera_data->getMainCamera())
	{
		return;
	}

	if (ImGui::BeginChild("Viewport"))
	{
		if (mColorBuffer == nullptr)
		{
			mColorBuffer = (TextureBuffer2D*)TextureMgr::getInstance()->findTexture("RB_Viewport");
		}

		if (mColorBuffer)
		{
			ImVec2 texture_size = ImVec2((float)Engine::getScreenWidth(), (float)Engine::getScreenHeight());
			ImVec2 display_size, offset, uv0, uv1;

			this->calculate(texture_size, ImGui::GetWindowSize(), display_size, offset, uv0, uv1);

			camera_data->getMainCamera()->setViewRect(0, 0, display_size.x, display_size.y);

			//Log::info(StringTool::stringFormat("Ratio[%.3f]", display_size.x / display_size.y));

			ImGui::SetCursorPos(offset);
			ImGui::Image((ImTextureID)mColorBuffer->getTextureID()
				, display_size
				, uv0
				, uv1);
		}

		ImGui::EndChild();
	}
}

void MyViewPortWindow::drawInfo(const ImVec2& pos)
{
	ImGui::SetNextWindowPos(pos);
	ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
	if (ImGui::Begin("状态(State)", 0, window_flags))
	{
		//gpu
		ImGui::Text("GPU: %s", Statistic::GPU);

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
		ImGui::Separator();

		ImGui::Text("Memory");
		ImGui::Text("Used: %.3f kb", Statistic::getMemoryUse() / 1024.0f);
		ImGui::Text("TotalID: %d", TinyGC::totalID());
		ImGui::SameLine();
		ImGui::Text("FreeID: %d", TinyGC::freeID());
		ImGui::SameLine();
		ImGui::Text("UsedID: %d", TinyGC::usedID());

		ImGui::Separator();

		//mouse
		ImGui::InputFloat2("MousePosition", glm::value_ptr(Statistic::mousePosition), "%.3f", ImGuiInputTextFlags_ReadOnly);
		ImGui::InputFloat2("MouseOffset", glm::value_ptr(Statistic::mouseOffset), "%.3f", ImGuiInputTextFlags_ReadOnly);

		ImGui::End();
	}
}

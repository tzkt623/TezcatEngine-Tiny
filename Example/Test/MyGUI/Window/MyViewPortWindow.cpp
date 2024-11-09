#include "MyViewPortWindow.h"

namespace tezcat::Editor
{

	TINY_EDITOR_WINDOW_INSTANCE_CPP(MyViewPortWindow);
	MyViewPortWindow::MyViewPortWindow()
		: GUIWindow("场景(Scene)")
		, mColorBuffer(nullptr)
		, mIsPlaying(true)
	{

	}

	MyViewPortWindow::~MyViewPortWindow()
	{
		TINY_EDITOR_WINDOW_DELETE_INSTACNE();
	}

	bool MyViewPortWindow::begin()
	{
		return ImGui::Begin(this->getName(), 0, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_MenuBar);
	}

	void MyViewPortWindow::calculate(const ImVec2& inTextureSize, const ImVec2& inWindowSize
		, ImVec2& outDisplaySize, ImVec2& outOffset, ImVec2& outUV0, ImVec2& outUV1)
	{
		outDisplaySize = inWindowSize;

		//按照视图比例 得到符合当前视图的窗口大小
		float ratio_xDy = inTextureSize.x / inTextureSize.y;
		float ratio_yDx = inTextureSize.y / inTextureSize.x;

		//先保证上下对齐,也就是需要计算左右偏移
		//获得当前比例下的宽度
		float width = inWindowSize.y * ratio_xDy;
		//如果宽度比当前的小,需要把图片移动到view中心
		if (width < inWindowSize.x)
		{
			//计算多出来的宽度的一半
			outOffset.x = (inWindowSize.x - width) * 0.5f;
			outDisplaySize.x = inWindowSize.y * ratio_xDy;
		}
		else
		{
			//宽度比当前的大
			//左右压缩满了
			//计算上下之间的空隙
			float height = inWindowSize.x * ratio_yDx;
			outOffset.y = (inWindowSize.y - height) * 0.5f;
			outDisplaySize.y = inWindowSize.x * ratio_yDx;
		}

		//计算当前显示大小与视图的比值
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
		GUIWindow::onRender();

		if (ImGui::BeginMenuBar())
		{
			static bool bInfo;
			ImGui::MenuItem("Info", 0, &bInfo);
			if (bInfo)
			{
				auto pos = ImGui::GetItemRectMin();
				pos.y += ImGui::GetItemRectSize().y;
				this->drawInfo(pos);
			}

			ImGui::EndMenuBar();
		}

		/*
		if (mIsPlaying)
		{
			if (ImGui::Button("Pause"))
			{
				mIsPlaying = false;
			}
		}
		else
		{
			if (ImGui::Button("Play"))
			{
				mIsPlaying = true;
			}
		}
		*/

		if (ImGui::BeginChild("Viewport"))
		{
			if (mColorBuffer == nullptr)
			{
				mColorBuffer = (Texture2D*)FrameBufferManager::getMainFrameBufferBuildin()->getAttachmentes().at(0);
			}

			if (mColorBuffer)
			{
				auto camera_data = CameraManager::getData();
				if (camera_data.lock() && camera_data->getMainCamera())
				{
					ImVec2 texture_size = ImVec2((float)mColorBuffer->getWidth(), (float)mColorBuffer->getHeight());
					ImVec2 display_size, offset, uv0, uv1;

					this->calculate(texture_size, ImGui::GetWindowSize(), display_size, offset, uv0, uv1);

					camera_data->getMainCamera()->setViewRect(0, 0, display_size.x, display_size.y);

					ImGui::SetCursorPos(offset);
					ImGui::Image((ImTextureID)mColorBuffer->getTextureID()
								, display_size
								, uv0
								, uv1);
				}
				else
				{
					ImVec2 texture_size = ImVec2((float)mColorBuffer->getWidth(), (float)mColorBuffer->getHeight());
					ImVec2 display_size, offset, uv0, uv1;

					this->calculate(texture_size, ImGui::GetWindowSize(), display_size, offset, uv0, uv1);

					ImGui::SetCursorPos(offset);
					ImGui::Image((ImTextureID)mColorBuffer->getTextureID()
								, display_size
								, ImVec2(0, 1)
								, ImVec2(1, 0));
				}

				if (ImGui::IsItemHovered())
				{
					InputSys::getInstance()->update();
				}
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
			ImGui::Text("GPU: %s", Profiler::GPU);

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
			ImGui::Separator();

			ImGui::Text("Memory");
			ImGui::Text("Used: %.3f mb", Profiler::getMemoryUse() / 1024.0f / 1024.0f);
			ImGui::Text("TotalID: %d", TinyGC::totalID());
			ImGui::SameLine();
			ImGui::Text("FreeID: %d", TinyGC::freeID());
			ImGui::SameLine();
			ImGui::Text("UsedID: %d", TinyGC::usedID());

			ImGui::Separator();

			//mouse
			ImGui::InputFloat2("MousePosition", glm::value_ptr(Profiler::mousePosition), "%.3f", ImGuiInputTextFlags_ReadOnly);
			ImGui::InputFloat2("MouseOffset", glm::value_ptr(Profiler::mouseOffset), "%.3f", ImGuiInputTextFlags_ReadOnly);

			ImGui::End();
		}
	}
}

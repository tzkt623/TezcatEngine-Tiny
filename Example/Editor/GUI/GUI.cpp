/*
	Copyright (C) 2024 Tezcat(特兹卡特) tzkt623@qq.com

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

#include "GUI.h"
#include "GUIWidget.h"
#include "../MyGUI/EditorContext.h"

namespace tezcat::Editor
{
	GUI::GUI()
	{

	}

	GUI::~GUI()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void GUI::init(GLFWwindow* window)
	{

		const char* glsl_version = "#version 410 core";
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.Fonts->AddFontFromFileTTF(
			"./Resource/font/deng.ttf",
			24.0f,
			nullptr,
			io.Fonts->GetGlyphRangesChineseFull());
		// 		io.Fonts->AddFontFromFileTTF(
		// 			"c:/windows/fonts/simhei.ttf",
		// 			16.0f,
		// 			nullptr,
		// 			io.Fonts->GetGlyphRangesChineseFull());
				//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
				//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		io.ConfigDockingWithShift = true;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(glsl_version);

		auto& style = ImGuizmo::GetStyle();
		style.HatchedAxisLineThickness = 2;

		style.TranslationLineArrowSize = 8;
		style.TranslationLineThickness = 8;

		style.RotationLineThickness = 8;
		style.RotationOuterLineThickness = 8;

		style.ScaleLineThickness = 8;
		style.ScaleLineCircleSize = 8;
	}

	void GUI::render()
	{
		if (!mInitList.empty())
		{
			for (auto widget : mInitList)
			{
				widget->init();
			}

			mInitList.clear();
		}

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
		EditorContext::beginFrame();

		auto it = mWindows.begin();
		auto end = mWindows.end();
		while (it != end)
		{
			if ((*it)->isClosed())
			{
				delete (*it);
				it = mWindows.erase(it);
				end = mWindows.end();
			}
			else
			{
				(*it)->update();
				++it;
			}
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		TINY_GL_CHECK("ImGUI");

		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}

		ImGuiHelper::clearInvalidDraging();
		EditorContext::endFrame();
	}

#pragma region Helper
	//-------------------------------------------------------------
//
//	MyDragDropController
//
	void ResourceDragDropHelper::dragData(const std::filesystem::path& path)
	{
		TINY_LOG_INFO(path.string());

		mIsDraging = true;
		mFilePath = path;
		mDragName.clear();

		auto ext = mFilePath.filename().extension().string();
		std::transform(ext.begin()
			, ext.end()
			, ext.begin()
			, [](unsigned char c) { return std::tolower(c); });

		mFileType = FileInfo::getFileType(ext);
		switch (mFileType)
		{
		case FileType::FT_None:
			break;
		case FileType::FT_Text_Begin:
			break;
		case FileType::FT_Text:
		case FileType::FT_Tysl:
		case FileType::FT_Tyin:
			mDragName = "Drag_Text";
			break;
		case FileType::FT_Text_End:
			break;

			//
		case FileType::FT_Img_Begin:
			break;
		case FileType::FT_Jpg:
		case FileType::FT_Png:
		case FileType::FT_Hdr:
			mDragName = "Drag_Image";
			break;
		case FileType::FT_Img_End:
			break;

			//
		case FileType::FT_Model_Begin:
			break;
		case FileType::FT_Fbx:
		case FileType::FT_Obj:
		case FileType::FT_PMX:
			mDragName = "Drag_Model";
			break;
		case FileType::FT_Model_End:
			break;

			//
		case FileType::FT_Unknown:
			break;
		default:
			break;
		}
	}

	std::tuple<bool, std::string> ResourceDragDropHelper::dropData()
	{
		//bool flag = !mDragName.empty();
		//std::string name(mDragName);

		return { !mDragName.empty(), mDragName };
	}

	ResourceDragDropHelper ImGuiHelper::ResDragDropHelper;

	void ImGuiHelper::fitImageToRect(const ImVec2& inTextureSize, const ImVec2& inWindowSize, ImVec2& outDisplaySize, ImVec2& outOffsetToCenter, ImVec2& outUV0, ImVec2& outUV1)
	{
		outDisplaySize = inWindowSize;

		//按照视图比例 得到符合当前视图的窗口大小
		float ratio_xDy = inTextureSize.x / inTextureSize.y;
		float ratio_yDx = inTextureSize.y / inTextureSize.x;

		//先保证上下对齐,也就是需要计算左右偏移(例如 600:600 16:9 1.8)
		//获得当前视口大小下,帧图片的理论宽度
		float width = inWindowSize.y * ratio_xDy; // 600*1.8 = 1080
		//如果宽度比当前的小,说明当前视口大小可以容纳当前帧
		//需要把帧图片移动到view中心
		if (width < inWindowSize.x)
		{
			//计算多出来的宽度的一半
			outOffsetToCenter.x = (inWindowSize.x - width) * 0.5f;
			outDisplaySize.x = width;
		}
		else
		{
			//宽度比当前的大,说明当前视口宽度就应该设置成帧图片的最大宽度
			//此时应该计算上下之间预留的空隙
			//以及把帧图片移动到视口正中间
			float height = inWindowSize.x * ratio_yDx;	//600*0.56=337.5
			outOffsetToCenter.y = (inWindowSize.y - height) * 0.5f;
			outDisplaySize.y = height;
		}

		//计算当前显示大小与视图的比值
		//
		outUV0 = ImVec2(0.0f, outDisplaySize.y / inTextureSize.y);
		outUV1 = ImVec2(outDisplaySize.x / inTextureSize.x, 0.0f);

		if (outUV0.y > 1.0f)
		{
			outUV0.y = 1.0f;
		}

		if (outUV1.x > 1.0f)
		{
			outUV1.x = 1.0f;
		}
	}

	void ImGuiHelper::fitImageToRect(const ImVec2& inWindowSize, const ImVec2& inImageSize, ImVec2& outDisplaySize, ImVec2& outOffsetToCenter)
	{
		float target_ratio = inWindowSize.x / inWindowSize.y;
		float image_ratio = inImageSize.x / inImageSize.y;

		// 计算缩放比例
		// 比如 16:9(17.7:10) > 16:10
		if (image_ratio >= target_ratio)
		{
			//此时image的宽超过了target的宽
			//需要用target的宽来计算出缩放后的image的高
			outDisplaySize.x = inWindowSize.x;
			outDisplaySize.y = (inImageSize.y / inImageSize.x) * inWindowSize.x;
			//将image移动的到正中央需要的偏移大小(垂直居中)
			outOffsetToCenter.y = (inWindowSize.y - outDisplaySize.y) * 0.5f;
		}
		//比如 16:10 < 16:9
		else
		{
			//此时image的高超过了target的高
			//需要用target的高来计算出缩放后的image的宽
			outDisplaySize.y = inWindowSize.y;
			outDisplaySize.x = (inImageSize.x / inImageSize.y) * inWindowSize.y;
			//将image移动的到正中央需要的偏移大小(水平居中)
			outOffsetToCenter.x = (inWindowSize.x - outDisplaySize.x) * 0.5f;
		}
	}

	void ImGuiHelper::itemBorderHighLight(ImU32 color /*= IM_COL32(255, 255, 0, 255)*/)
	{
		ImGui::GetWindowDrawList()->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), color, 0, 0, 3); // 3px 黄色边框
	}

	void ImGuiHelper::itemHighLight(ImU32 color /*= IM_COL32(255, 255, 0, 255)*/)
	{
		ImGui::GetWindowDrawList()->AddRectFilled(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), color); // 半透明黄色
	}

	void ImGuiHelper::dragResource(const file_path& path)
	{
		if (!ResDragDropHelper.mIsDraging && ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
		{
			ResDragDropHelper.mIsDraging = true;
			ResDragDropHelper.dragData(path);
			ImGui::SetDragDropPayload(ResDragDropHelper.mDragName.c_str(), nullptr, 0);
			ImGui::EndDragDropSource();
		}
	}

	void ImGuiHelper::dropResource(const std::function<void(file_path)>& function)
	{
		if (ResDragDropHelper.mIsDraging && ImGui::BeginDragDropTarget())
		{
			if (ImGui::AcceptDragDropPayload(ResDragDropHelper.mDragName.c_str()))
			{
				ResDragDropHelper.mIsDraging = false;
				function(ResDragDropHelper.mFilePath);
			}

			ImGui::EndDragDropTarget();
		}
	}

	void ImGuiHelper::clearInvalidDraging()
	{
		if (ResDragDropHelper.mIsDraging && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
		{
			ResDragDropHelper.mIsDraging = false;
		}
	}

	void ImGuiHelper::rect(int32_t id, const ImVec2& size, ImU32 color)
	{
		// 获取当前窗口的 DrawList
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
		{
			return;
		}

		// 生成唯一 ID（基于父级 ID 和用户提供的字符串）
		ImGuiID item_id = window->GetID(id);

		auto& cpos = window->DC.CursorPos;
		// 定义矩形的边界框（屏幕坐标）
		ImRect rect(cpos, ImVec2(cpos.x + size.x, cpos.y + size.y));

		// 将矩形注册为可交互项（继承 ImGui 的交互管理）
		if (!ImGui::ItemAdd(rect, item_id))
		{
			return;
		}

		//// 检测交互状态（悬停、点击等）
		//bool is_hovered = ImGui::IsItemHovered();
		//bool is_clicked = ImGui::IsItemClicked();
		//
		//// 根据状态改变颜色
		//if (is_hovered)
		//{
		//	color = ImGui::GetColorU32(ImGuiCol_ButtonHovered);
		//}
		//
		//if (is_clicked)
		//{
		//	color = ImGui::GetColorU32(ImGuiCol_ButtonActive);
		//}

		// 绘制矩形
		ImGui::GetWindowDrawList()->AddRect(rect.Min, rect.Max, color);
	}

	void ImGuiHelper::toggleButton(const char* str_id, bool* v)
	{
		ImVec4* colors = ImGui::GetStyle().Colors;
		ImVec2 p = ImGui::GetCursorScreenPos();
		ImDrawList* draw_list = ImGui::GetWindowDrawList();

		float height = ImGui::GetFrameHeight();
		float width = height * 1.55f;
		float radius = height * 0.50f;
		float rounding = 0.2f;

		ImGui::InvisibleButton(str_id, ImVec2(width, height));
		if (ImGui::IsItemClicked())
		{
			*v = !*v;
		}
		ImGuiContext& gg = *GImGui;
		float ANIM_SPEED = 0.085f;
		if (gg.LastActiveId == gg.CurrentWindow->GetID(str_id))// && g.LastActiveIdTimer < ANIM_SPEED)
		{
			float t_anim = ImSaturate(gg.LastActiveIdTimer / ANIM_SPEED);
		}
		if (ImGui::IsItemHovered())
		{
			draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), ImGui::GetColorU32(*v ? colors[ImGuiCol_ButtonActive] : ImVec4(0.78f, 0.78f, 0.78f, 1.0f)), height * rounding);
		}
		else
		{
			draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), ImGui::GetColorU32(*v ? colors[ImGuiCol_Button] : ImVec4(0.85f, 0.85f, 0.85f, 1.0f)), height * rounding);
		}

		ImVec2 center = ImVec2(radius + (*v ? 1 : 0) * (width - radius * 2.0f), radius);
		draw_list->AddRectFilled(ImVec2((p.x + center.x) - 9.0f, p.y + 1.5f)
			, ImVec2((p.x + (width / 2) + center.x) - 9.0f, p.y + height - 1.5f)
			, IM_COL32(255, 255, 255, 255)
			, height * rounding);
	}

#pragma endregion Helper
}

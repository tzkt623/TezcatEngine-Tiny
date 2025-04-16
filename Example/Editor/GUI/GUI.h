#pragma once
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

#include "include/Tiny.h"
#include "../ThirdParty/imgui/imgui.h"
#include "../ThirdParty/imgui/imgui_internal.h"
#include "../ThirdParty/imgui/imgui_impl_glfw.h"
#include "../ThirdParty/imgui/imgui_impl_opengl3.h"
#include "../ThirdParty/imgui/TextEditor/TextEditor.h"
#include "../ThirdParty/imguizmo/ImGuizmo.h"

#include "../ThirdParty/nfd/include/nfd.h"


struct GLFWwindow;

namespace tezcat::Editor
{
	class GUIWindow;
	class GUI
	{
	public:
		GUI();
		~GUI();
		void render();

	public:
		void init(GLFWwindow* window);

		void addWindow(GUIWindow* window)
		{
			mInitList.push_back(window);
			mWindows.push_back(window);
		}

	private:
		std::vector<GUIWindow*> mInitList;
		std::list<GUIWindow*> mWindows;
	};


	class ResourceDragDropHelper
	{
	public:
		void dragData(const std::filesystem::path& path);
		std::tuple<bool, std::string> dropData();
		std::string getFilePath() { return mFilePath.string(); }

	public:
		std::string mDragName;
		std::filesystem::path mFilePath;
		bool mIsDraging;

	private:
		FileType mFileType;
	};

	class ImGuiHelper
	{
	public:
		static void fitImageToRect(const ImVec2& inTextureSize, const ImVec2& inWindowSize
			, ImVec2& outDisplaySize, ImVec2& outOffsetToCenter, ImVec2& outUV0, ImVec2& outUV1);

		static void fitImageToRect(const ImVec2& inWindowSize, const ImVec2& inImageSize, ImVec2& outDisplaySize, ImVec2& outOffsetToCenter);

		static void itemBorderHighLight(ImU32 color = IM_COL32(0, 255, 0, 255));
		static void itemHighLight(ImU32 color = IM_COL32(56, 150, 255, 150));

		static void rect(int32_t id, const ImVec2& size, ImU32 color = IM_COL32(0, 255, 0, 255));


		template<class... Args>
		static void textWithBG(std::format_string<Args...> fmt, Args&&... args)
		{
			ImVec4* colors = ImGui::GetStyle().Colors;
			auto& space = ImGui::GetStyle().ItemSpacing;
			auto& frame = ImGui::GetStyle().FramePadding;

			auto s = std::vformat(fmt.get(), std::make_format_args(args...));
			auto size = ImGui::CalcTextSize(s.c_str());

			auto pos = ImGui::GetItemRectMin();

			ImVec2 now_min(pos.x + space.x - frame.x, pos.y + space.y - frame.y);
			ImVec2 now_max(now_min.x + size.x + frame.x, now_min.y + size.y + frame.y);

			ImGui::GetWindowDrawList()->AddRect(now_min
				, now_max
				, ImGui::GetColorU32(colors[ImGuiCol_Button]));
			ImGui::SetCursorPos(pos);
			ImGui::Text(s.c_str());
		}

		static void toggleButton(const char* str_id, bool* v);

		//DragDrop
	public:
		static void dragResource(const file_path& path);
		static void dropResource(const std::function<void(file_path)>& function);
		static void clearInvalidDraging();

	public:


	private:
		static ResourceDragDropHelper ResDragDropHelper;
	};

	// 加法
	inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs)
	{
		return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y);
	}

	inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs)
	{
		return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y);
	}

	// 标量乘法
	inline ImVec2 operator*(const ImVec2& v, float scalar)
	{
		return ImVec2(v.x * scalar, v.y * scalar);
	}

	// 点积
	inline float dot(const ImVec2& a, const ImVec2& b)
	{
		return a.x * b.x + a.y * b.y;
	}

	// 向量长度
	inline float length(const ImVec2& v)
	{
		return sqrtf(v.x * v.x + v.y * v.y);
	}
}

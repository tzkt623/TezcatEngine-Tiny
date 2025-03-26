#pragma once

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

		static void rect(int32_t id, const ImVec2& size, ImU32 color);


		template<class... Args>
		static void textWithBG(std::format_string<Args...> fmt, Args&&... args)
		{
			auto s = std::vformat(fmt.get(), std::make_format_args(args...));
			auto size = ImGui::CalcTextSize(s.c_str());

			auto& space = ImGui::GetStyle().ItemSpacing;
			auto& frame = ImGui::GetStyle().FramePadding;

			auto pos = ImGui::GetCursorPos();
			ImVec2 now_min(ImGui::GetItemRectMax().x + space.x - frame.x, ImGui::GetItemRectMin().y + space.y - frame.y);
			ImVec2 now_max(now_min.x + size.x + frame.x, now_min.y + size.y + frame.y);

			ImGui::GetWindowDrawList()->AddRectFilled(now_min
				, now_max
				, ImColor(255, 100, 100, 255));
			ImGui::SetCursorPos(pos);
			ImGui::Text(s.c_str());
		}

		//DragDrop
	public:
		static void dragResource(const file_path& path);
		static void dropResource(const std::function<void(file_path)>& function);
		static void clearInvalidDraging();

	private:
		static ResourceDragDropHelper ResDragDropHelper;
	};
}

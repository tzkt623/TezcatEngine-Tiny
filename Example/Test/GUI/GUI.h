#pragma once

#include "include/Tiny.h"
#include "../ThirdParty/imgui/imgui.h"
#include "../ThirdParty/imgui/imgui_impl_glfw.h"
#include "../ThirdParty/imgui/imgui_impl_opengl3.h"
#include "../ThirdParty/imgui/TextEditor/TextEditor.h"

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

		//DragDrop
	public:
		static void dragResource(const file_path& path);
		static void dropResource(const std::function<void(file_path)>& function);
		static void clearInvalidDraging();

	private:
		static ResourceDragDropHelper ResDragDropHelper;
	};
}

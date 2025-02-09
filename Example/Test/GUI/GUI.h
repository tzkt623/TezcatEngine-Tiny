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

		void removeWindow(GUIWindow* window)
		{
			auto it = mWindows.begin();
			while (it != mWindows.end())
			{
				if ((*it) == window)
				{
					mWindows.erase(it);
					break;
				}
				it++;
			}
		}

	private:
		std::vector<GUIWindow*> mInitList;
		std::list<GUIWindow*> mWindows;
	};

}

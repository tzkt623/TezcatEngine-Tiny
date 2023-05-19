#pragma once

#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"
#include "../Tool/Tool.h"


struct GLFWwindow;
namespace tezcat::Tiny
{
	class GUIWindow;
	class TINY_API GUI
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




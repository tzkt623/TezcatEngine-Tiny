#pragma once

#include "../Head/CppHead.h"
#include "Utility/Tools.h"

namespace tezcat::Tiny::Core
{
	class Engine;
	class GUIWindow;
	class GUI
	{
	public:
		GUI();
		~GUI();
		void render();

	public:
		void init(Engine* engine);

		void addWindow(GUIWindow* window)
		{
			m_InitList.push_back(window);

			m_Windows.push_back(window);
		}

		void removeWindow(GUIWindow* window)
		{
			auto it = m_Windows.begin();
			while (it != m_Windows.end())
			{
				if ((*it) == window)
				{
					m_Windows.erase(it);
					break;
				}
				it++;
			}
		}

	private:
		std::vector<GUIWindow*> m_InitList;
		std::vector<GUIWindow*> m_Windows;
		Engine* m_Engine;
	};
}




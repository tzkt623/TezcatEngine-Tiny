/*
	Copyright (C) 2022 - 2025 Tezcat(特兹卡特) tzkt623@qq.com

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

struct GLFWwindow;

namespace tezcat::Editor
{
	class EditorMainWindow;
	class GUI;
	/// <summary>
	/// engine负责所有window相关的操作
	/// graphics只负责draw
	///
	///	editor初始化
	/// editor开始渲染
	/// 并不启动engine的内容
	/// 
	/// </summary>
	class TINY_API WindowsEditor : public Engine
	{
	protected:
		bool preInit() override;
		bool onInit() override;
		bool postInit() override;
		void beforeLoop() override;
		void endLoop() override;
		void preUpdate() override;
		void postUpdate() override;
		void onUpdate() override;

		void notifyRender() override;

		BaseGraphics* createGraphics() override;

	private:
		GLFWwindow* mWindow;
		GUI* mGUI;
		EditorMainWindow* mEditorWindow;

		double mTimeOld;
		double mTimeNow;
	};
}

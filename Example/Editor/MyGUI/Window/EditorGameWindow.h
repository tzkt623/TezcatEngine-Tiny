#pragma once
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

#include "../EditorContext.h"

namespace tezcat::Editor
{
	class EditorGameWindow : public GUIWindow
	{
		TINY_EDITOR_WINDOW_INSTANCE_H(EditorGameWindow)

	protected:
		bool begin() override;
		void onRender() override;

	private:
		Texture2D* mColorBuffer;
		std::vector<float> mFrameTimeBuffer;
		bool mIsPlaying;
		ImVec2 mViewPortPos;
		ImVec2 mMousePos;
		ImVec2 mFramePos;
	};
}

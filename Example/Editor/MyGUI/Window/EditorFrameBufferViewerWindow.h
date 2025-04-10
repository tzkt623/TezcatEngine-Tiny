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

#include "../EditorContext.h"
namespace tezcat::Editor
{
	class EditorFrameBufferViewerWindow : public GUIWindow
	{
		TINY_EDITOR_WINDOW_INSTANCE_H(EditorFrameBufferViewerWindow)
	public:
		void init() override;
		void onRender() override;

	private:
		FrameBuffer* mSelectBuffer;
	};
}

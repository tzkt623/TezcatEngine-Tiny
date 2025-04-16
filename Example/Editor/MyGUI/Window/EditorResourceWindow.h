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
	class EditorResourceWindow : public GUIWindow
	{
		TINY_EDITOR_WINDOW_INSTANCE_H(EditorResourceWindow)


	public:
		bool begin() override;
		void end() override;

		uint32_t loadIcon(const std::string& filePath);

	protected:
		void onRender() override;
		void drawFolder();
		void drawDirectory(std::filesystem::directory_entry& entry);
		void drawFile(const std::filesystem::path& path, int32_t id);

		void itemDoubleClick(const std::filesystem::path& path);
		void itemDrag(const std::filesystem::path& path);

		void drawCreateMenu();

	private:
		std::string mRootPath;
		std::filesystem::directory_entry mDirectoryEntry;
		std::filesystem::directory_entry mFolderEntry;
		std::filesystem::path mPath;
		std::filesystem::path mCurrentPath;

		std::vector<Texture2D*> mIconAry;
		std::vector<std::filesystem::path> mPathAry;

		int mFileItemSize;
		bool mIsDraging;

		ImVec2 mUVx;
		ImVec2 mUVy;
	};

}

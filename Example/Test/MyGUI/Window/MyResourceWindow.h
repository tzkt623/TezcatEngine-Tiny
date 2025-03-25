#pragma once

#include "../MyGUIContext.h"
namespace tezcat::Editor
{
	class MyResourceWindow : public GUIWindow
	{
		TINY_EDITOR_WINDOW_INSTANCE_H(MyResourceWindow)


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

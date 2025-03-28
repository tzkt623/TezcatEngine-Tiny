﻿#include "MyResourceWindow.h"
#include "../../MyEvent.h"
namespace tezcat::Editor
{
	uint32_t Icon_DefaultFile = 0;
	uint32_t Icon_Folder = 0;
	uint32_t Icon_Txt = 0;
	uint32_t Icon_Png = 0;
	uint32_t Icon_Jpg = 0;
	uint32_t Icon_Fbx = 0;

	TINY_EDITOR_WINDOW_INSTANCE_CPP(MyResourceWindow);
	MyResourceWindow::MyResourceWindow()
		: GUIWindow("资源(Resource)")
		, mFileItemSize(64)
		, mUVx(0, 1)
		, mUVy(1, 0)
	{
		mRootPath = "./Resource";
		mCurrentPath = "./Resource";
		mDirectoryEntry.assign(mRootPath);
		Icon_DefaultFile = this->loadIcon("Editor/Icon/DefaultFile.png");
		Icon_Folder = this->loadIcon("Editor/Icon/Folder.png");
		Icon_Txt = this->loadIcon("Editor/Icon/Txt.png");
		Icon_Png = this->loadIcon("Editor/Icon/Png.png");
		Icon_Jpg = this->loadIcon("Editor/Icon/Jpg.png");
		Icon_Fbx = this->loadIcon("Editor/Icon/Fbx.png");
	}

	MyResourceWindow::~MyResourceWindow()
	{
		TINY_EDITOR_WINDOW_DELETE_INSTACNE();
	}

	uint32_t MyResourceWindow::loadIcon(const std::string& filePath)
	{
		auto tex = ResourceManager::loadOnly<Texture2D>(filePath);
		tex->saveObject();
		mIconAry.push_back(tex);
		return mIconAry.size() - 1;
	}

	bool MyResourceWindow::begin()
	{
		return ImGui::Begin(mName.c_str(), &mIsOpen, ImGuiWindowFlags_NoScrollbar);
	}

	void MyResourceWindow::end()
	{
		ImGui::End();
	}

	void MyResourceWindow::onRender()
	{
		GUIWindow::onRender();

		mDirectoryEntry.assign(mRootPath);

		if (ImGui::BeginTable("##ResourceWindow0", 2, ImGuiTableFlags_Resizable))
		{
			ImGui::TableNextColumn();
			ImGui::BeginChild("##目录(Directory)");
			this->drawDirectory(mDirectoryEntry);
			ImGui::EndChild();

			ImGui::TableNextColumn();
			ImGui::BeginChild("##文件夹(Folder)");
			this->drawFolder();
			ImGui::EndChild();

			ImGui::EndTable();
		}
	}

	void MyResourceWindow::drawFolder()
	{
		static int space = 32;
		static int path_button_width = 64;

		float item_space = mFileItemSize + space;
		float window_length = ImGui::GetWindowSize().x;
		int item_count = window_length / item_space;
		if (item_count < 1)
		{
			item_count = 1;
		}

		int path_button_length = 0;
		std::filesystem::path temp = mCurrentPath;
		while (temp.has_parent_path())
		{
			path_button_length += path_button_width;
			mPathAry.emplace_back(temp);
			temp = temp.parent_path();
		}

		int path_length = window_length - 120;
		if (path_length < 0)
		{
			path_length = 0;
		}

		int path_count = path_length / path_button_width;
		if (path_count >= mPathAry.size())
		{
			for (int i = mPathAry.size() - 1; i >= 0; i--)
			{
				if (ImGui::Button(mPathAry[i].filename().string().c_str(), ImVec2(path_button_width, 0)))
				{
					mCurrentPath = mPathAry[i];
				}
				ImGui::SameLine();
			}
		}
		else
		{
			for (int i = mPathAry.size() - 1; i >= path_count; i--)
			{
				if (ImGui::Button(mPathAry[i].filename().string().c_str(), ImVec2(path_button_width, 0)))
				{
					mCurrentPath = mPathAry[i];
				}
				ImGui::SameLine();
			}
		}
		mPathAry.clear();
		ImGui::SetNextItemWidth(120);
		ImGui::SliderInt("##ResourceWindowItemSize", &mFileItemSize, 64, 256);

		if (ImGui::BeginTable("##ResourceWindow1", item_count))
		{
			//ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 1.0f, 1.0f, 0.0f));
			//ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(1.0f, 1.0f, 1.0f, 0.0f));

			std::vector<std::filesystem::path> files;
			mDirectoryEntry.assign(mCurrentPath);
			std::filesystem::directory_iterator list(mDirectoryEntry);
			int32_t id = 0;
			for (auto& it : list)
			{
				switch (it.status().type())
				{
				case std::filesystem::file_type::directory:
				{
					ImGui::TableNextColumn();

					ImGui::PushID(id++);
					ImGui::Image((ImTextureID)mIconAry[Icon_Folder]->getTextureID()
						, ImVec2(mFileItemSize, mFileItemSize)
						, mUVx
						, mUVy);
					if (ImGui::IsItemHovered())
					{
						ImGuiHelper::itemHighLight();

						if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
						{
							mCurrentPath = it.path();
						}
					}
					ImGui::PopID();

					ImGui::SetNextItemWidth(mFileItemSize);
					ImGui::TextWrapped(it.path().filename().string().c_str());
					break;
				}
				case std::filesystem::file_type::regular:
					files.emplace_back(it.path());
					break;
				}
			}

			for (auto& path : files)
			{
				ImGui::TableNextColumn();
				this->drawFile(path, id++);
			}

			//ImGui::PopStyleColor();
			ImGui::EndTable();
		}
	}

	void MyResourceWindow::drawFile(const std::filesystem::path& path, int32_t id)
	{
		//ImGui::PushID(id);
		ImGui::Image((ImTextureID)mIconAry[Icon_DefaultFile]->getTextureID()
			, ImVec2(mFileItemSize, mFileItemSize)
			, mUVx
			, mUVy);


		if (ImGui::IsItemHovered())
		{
			ImGuiHelper::dragResource(path);

			//this->itemDrag(path);

			ImGuiHelper::itemHighLight();

			if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				this->itemDoubleClick(path);
			}
			else if (ImGui::GetDragDropPayload() == nullptr && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
			{

			}
		}
		//ImGui::PopID();

		ImGui::SetNextItemWidth(mFileItemSize);
		ImGui::TextWrapped(path.filename().string().c_str());
	}

	void MyResourceWindow::drawDirectory(std::filesystem::directory_entry& entry)
	{
		if (entry.is_directory())
		{
			static ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth
				| ImGuiTreeNodeFlags_OpenOnArrow
				| ImGuiTreeNodeFlags_OpenOnDoubleClick;
			auto& path = entry.path();
			if (ImGui::TreeNodeEx(path.filename().string().c_str(), flags))
			{
				if (ImGui::IsItemClicked())
				{
					mCurrentPath = path;
				}

				std::filesystem::directory_iterator list(path);
				for (auto& it : list)
				{
					entry.assign(it.path());
					if (ImGui::IsItemClicked())
					{
						mFolderEntry = entry;
					}
					this->drawDirectory(entry);
				}

				ImGui::TreePop();
			}
		}
	}

	void MyResourceWindow::itemDoubleClick(const std::filesystem::path& path)
	{
		auto ext = path.filename().extension().string();
		std::transform(ext.begin()
			, ext.end()
			, ext.begin()
			, [](unsigned char c) { return std::tolower(c); });

		auto type = FileTool::getFileType(ext);
		switch (type)
		{
		case FileType::FT_None:
			break;
		case FileType::FT_Text_Begin:
			break;
		case FileType::FT_Text:
		{
			break;
		}
		case FileType::FT_Tysl:
		{
			auto file = path;
			MyEvent::get()->dispatch({ MyEventID::Window_OpenShaderEditor , &file });
			break;
		}
		case FileType::FT_Tyin:
		{
			auto file = path;
			MyEvent::get()->dispatch({ MyEventID::Window_OpenShaderEditor , &file });
			break;
		}
		case FileType::FT_Text_End:
			break;
		case FileType::FT_Img_Begin:
			break;
		case FileType::FT_Jpg:
		case FileType::FT_Png:
		case FileType::FT_Hdr:
		{
			auto file = path;
			MyEvent::get()->dispatch({ MyEventID::Window_OpenImageViwer , &file });
			break;
		}
		case FileType::FT_Img_End:
			break;
		case FileType::FT_Unknown:
			break;
		default:
			break;
		}
	}

	void MyResourceWindow::itemDrag(const std::filesystem::path& path)
	{
		ImGuiHelper::dragResource(path);

		//if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
		//{
		//	auto drag_name = MyGUIContext::DragDropController.dragData(path);
		//	ImGui::SetDragDropPayload(drag_name.data(), nullptr, 0);
		//	ImGui::EndDragDropSource();
		//}
	}
}

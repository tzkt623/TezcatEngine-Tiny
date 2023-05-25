#include "MyResourceWindow.h"
#include "../../MyEvent.h"

CreateInstanceCPP(MyResourceWindow);
MyResourceWindow::MyResourceWindow()
	: GUIWindow("资源(Resource)")
{
	mRootPath = ".\\Resource";
	mEntry.assign(mRootPath);
}

MyResourceWindow::~MyResourceWindow()
{
	DeleteInstance();
}

bool MyResourceWindow::begin()
{
	return ImGui::Begin(mName.c_str(), &mIsOpen);
}

void MyResourceWindow::end()
{
	ImGui::End();
}

void MyResourceWindow::onRender()
{
	GUIWindow::onRender();

	mEntry.assign(mRootPath);
	this->drawTree(mEntry);
}

void MyResourceWindow::drawTree(std::filesystem::directory_entry& entry)
{
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth
		| ImGuiTreeNodeFlags_OpenOnArrow;

	auto& path = entry.path();
	switch (entry.status().type())
	{
	case std::filesystem::file_type::directory:
	{
		if (ImGui::TreeNodeEx(path.filename().string().c_str(), flags))
		{
			std::filesystem::directory_iterator list(path);
			for (auto& it : list)
			{
				entry.assign(it.path());
				this->drawTree(entry);
			}

			ImGui::TreePop();
		}
		break;
	}
	case std::filesystem::file_type::regular:
	{
		flags |= ImGuiTreeNodeFlags_Leaf;
		if (ImGui::TreeNodeEx(path.filename().string().c_str(), flags))
		{
			if (ImGui::IsItemClicked() && ImGui::IsMouseDoubleClicked(0))
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
					break;
				case FileType::FT_Png:
					break;
				case FileType::FT_Hdr:
					break;
				case FileType::FT_Img_End:
					break;
				case FileType::FT_Unknown:
					break;
				default:
					break;
				}
			}

			ImGui::TreePop();
		}


		break;
	}
	default:
		break;
	}
}

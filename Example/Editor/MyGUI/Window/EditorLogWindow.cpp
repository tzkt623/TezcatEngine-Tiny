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
#include "EditorLogWindow.h"

namespace tezcat::Editor
{
	TINY_EDITOR_WINDOW_INSTANCE_CPP(EditorLogWindow);
	EditorLogWindow::EditorLogWindow()
		: GUIWindow("记录(Log)")
	{
		mAutoScroll = true;
		this->clear();
		//Log::setImp(this);
	}

	EditorLogWindow::~EditorLogWindow()
	{
		TINY_EDITOR_WINDOW_DELETE_INSTACNE();
	}

	void EditorLogWindow::onRender()
	{
		GUIWindow::onRender();
		// Options menu
	// 	if (ImGui::BeginPopup("Options"))
	// 	{
	// 		ImGui::Checkbox("Auto-scroll", &mAutoScroll);
	// 		ImGui::EndPopup();
	// 	}

		// Main window
	// 	if (ImGui::Button("Options"))
	// 	{
	// 		ImGui::OpenPopup("Options");
	//  }
		auto& logs = Log::allLog();
		ImGui::Checkbox("Auto-scroll", &mAutoScroll);

		static bool mShowEngine = true, mShowInfo = true, mShowError = true, mShowWarning = true;
		ImGui::SameLine();
		ImGui::Checkbox("Engine", &mShowEngine);
		ImGui::SameLine();
		ImGui::Checkbox("Error", &mShowError);
		ImGui::SameLine();
		ImGui::Checkbox("Info", &mShowInfo);
		ImGui::SameLine();
		ImGui::Checkbox("Warning", &mShowWarning);

		ImGui::SameLine();
		bool clear = ImGui::Button("Clear");
		ImGui::SameLine();
		bool copy = ImGui::Button("Copy");
		//ImGui::SameLine();
		//mFilter.Draw("Filter", -100.0f);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(120);
		ImGui::LabelText("##数量(Count)", std::to_string(logs.size()).c_str());
		ImGui::SameLine();


		ImGui::Separator();

		ImGui::BeginChild("##Scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
		if (clear)
		{
			Log::clear();

			//this->clear();
		}
		if (copy)
		{
			ImGui::LogToClipboard();
		}

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

		static ImVec4 engine(0.0f, 1.0f, 0.0f, 1.0f);
		static ImVec4 info(1.0f, 1.0f, 1.0f, 1.0f);
		static ImVec4 error(1.0f, 0.0f, 0.0f, 1.0f);
		static ImVec4 warning(0.0f, 1.0f, 1.0f, 1.0f);

		ImGuiListClipper clipper;
		clipper.Begin(logs.size(), ImGui::GetTextLineHeightWithSpacing());
		while (clipper.Step())
		{
			for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
			{
				auto data = logs[line_no];
				switch (data->type)
				{
				case LogTypeID::LT_Engine:
					if (mShowEngine)
					{
						ImGui::TextColored(engine, data->str.c_str());
					}
					break;
				case LogTypeID::LT_Info:
					if (mShowInfo)
					{
						ImGui::TextColored(info, data->str.c_str());
					}
					break;
				case LogTypeID::LT_Error:
					if (mShowError)
					{
						ImGui::TextColored(error, data->str.c_str());
					}
					break;
				case LogTypeID::LT_Warning:
					if (mShowWarning)
					{
						ImGui::TextColored(warning, data->str.c_str());
					}
					break;
				default:
					break;
				}
			}
		}
		clipper.End();

		ImGui::PopStyleVar();

		//if (Log::isDirty() && mAutoScroll && ImGui::GetScrollY() <= ImGui::GetScrollMaxY())
		if (Log::isDirty() && mAutoScroll)
		{
			Log::clearDirty();
			ImGui::SetScrollHereY(0.0f);
		}

		ImGui::EndChild();
	}

	void EditorLogWindow::clear()
	{
		mTextBuffer.clear();
		mLineOffsets.clear();
		mLineOffsets.push_back(0);
	}

	void EditorLogWindow::addLog(const char* fmt, ...) IM_FMTARGS(2)
	{
		int old_size = mTextBuffer.size();
		va_list args;
		va_start(args, fmt);
		mTextBuffer.appendfv(fmt, args);
		va_end(args);
		for (int new_size = mTextBuffer.size(); old_size < new_size; old_size++)
		{
			if (mTextBuffer[old_size] == '\n')
			{
				mLineOffsets.push_back(old_size + 1);
			}
		}
	}

	void EditorLogWindow::addLog(const std::string& info)
	{
		int old_size = mTextBuffer.size();
		mTextBuffer.append(info.c_str());
		for (int new_size = mTextBuffer.size(); old_size < new_size; old_size++)
		{
			if (mTextBuffer[old_size] == '\n')
			{
				mLineOffsets.push_back(old_size + 1);
			}
		}
	}
}

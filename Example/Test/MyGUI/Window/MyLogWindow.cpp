#include "MyLogWindow.h"


CreateInstanceCPP(MyLogWindow);
MyLogWindow::MyLogWindow()
	: GUIWindow("记录(Log)")
{
	mAutoScroll = true;
	this->clear();
	//Log::setImp(this);
}

MyLogWindow::~MyLogWindow()
{
	DeleteInstance();
}
/*
void MyLogWindow::onRender()
{
	// Options menu
	if (ImGui::BeginPopup("Options"))
	{
		ImGui::Checkbox("Auto-scroll", &mAutoScroll);
		ImGui::EndPopup();
	}

	// Main window
	if (ImGui::Button("Options"))
	{
		ImGui::OpenPopup("Options");
	}
	ImGui::SameLine();
	bool clear = ImGui::Button("Clear");
	ImGui::SameLine();
	bool copy = ImGui::Button("Copy");
	ImGui::SameLine();
	mFilter.Draw("Filter", -100.0f);

	ImGui::Separator();
	ImGui::BeginChild("Scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

	if (clear)
	{
		this->clear();
	}
	if (copy)
	{
		ImGui::LogToClipboard();
	}

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
	const char* buf = mTextBuffer.begin();
	const char* buf_end = mTextBuffer.end();
	if (mFilter.IsActive())
	{
		for (int line_no = 0; line_no < mLineOffsets.Size; line_no++)
		{
			const char* line_start = buf + mLineOffsets[line_no];
			const char* line_end = (line_no + 1 < mLineOffsets.Size) ? (buf + mLineOffsets[line_no + 1] - 1) : buf_end;
			if (mFilter.PassFilter(line_start, line_end))
			{
				//ImGui::TextUnformatted(line_start, line_end);
				ImGui::TextUnformatted(line_start, line_end);
			}
		}
	}
	else
	{
		ImGuiListClipper clipper;
		clipper.Begin(mLineOffsets.Size);
		while (clipper.Step())
		{
			for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
			{
				const char* line_start = buf + mLineOffsets[line_no];
				const char* line_end = (line_no + 1 < mLineOffsets.Size) ? (buf + mLineOffsets[line_no + 1] - 1) : buf_end;
				//ImGui::TextUnformatted(line_start, line_end);
			}
		}
		clipper.End();
	}
	ImGui::PopStyleVar();

	if (mAutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
	{
		ImGui::SetScrollHereY(1.0f);
	}

	ImGui::EndChild();
	//ImGui::End();
}
*/

void MyLogWindow::onRender()
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

	ImGui::LabelText("数量(Count)", std::to_string(logs.size()).c_str());
	ImGui::SameLine();
	ImGui::Checkbox("Auto-scroll", &mAutoScroll);

	static bool mOnlyEngine, mOnlyInfo, mOnlyError, mOnlyWarning;
	ImGui::SameLine();
	ImGui::Checkbox("Engine", &mOnlyEngine);
	ImGui::SameLine();
	ImGui::Checkbox("Error", &mOnlyError);
	ImGui::SameLine();
	ImGui::Checkbox("Info", &mOnlyInfo);
	ImGui::SameLine();
	ImGui::Checkbox("Warning", &mOnlyWarning);

	ImGui::SameLine();
	bool clear = ImGui::Button("Clear");
	ImGui::SameLine();
	bool copy = ImGui::Button("Copy");
	//ImGui::SameLine();
	//mFilter.Draw("Filter", -100.0f);

	ImGui::Separator();
	ImGui::BeginChild("Scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

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
	clipper.Begin(logs.size());
	while (clipper.Step())
	{
		for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
		{
			auto data = logs[line_no];
			switch (data->type)
			{
			case LogTypeID::LT_Engine:
				ImGui::TextColored(engine, data->str.c_str());
				break;
			case LogTypeID::LT_Info:
				ImGui::TextColored(info, data->str.c_str());
				break;
			case LogTypeID::LT_Error:
				ImGui::TextColored(error, data->str.c_str());
				break;
			case LogTypeID::LT_Warning:
				ImGui::TextColored(warning, data->str.c_str());
				break;
			default:
				break;
			}
		}
	}
	clipper.End();

	ImGui::PopStyleVar();

	if (mAutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
	{
		ImGui::SetScrollHereY(1.0f);
	}

	ImGui::EndChild();
	//ImGui::End();
}

void MyLogWindow::clear()
{
	mTextBuffer.clear();
	mLineOffsets.clear();
	mLineOffsets.push_back(0);
}

void MyLogWindow::addLog(const char* fmt, ...) IM_FMTARGS(2)
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

void MyLogWindow::addLog(const std::string& info)
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

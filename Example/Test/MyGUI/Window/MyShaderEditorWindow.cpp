#include "MyShaderEditorWindow.h"

namespace tezcat::Editor
{
	std::unordered_map<std::filesystem::path, MyShaderEditorWindow*> MyShaderEditorWindow::sShaderEditorRegister;

	MyShaderEditorWindow::MyShaderEditorWindow()
		: GUIWindow("ShaderEditor")
		, mIsShader(false)
	{
		std::vector<const char*> uniforms;
		uniforms.reserve(UniformID::allStringCount());
		for (auto i = 0; i < UniformID::allStringCount(); i++)
		{
			uniforms.push_back(UniformID::getStringStatic(i).data());
		}

		uniforms.push_back("gl_Position");
		uniforms.push_back("gl_FragColor");
		uniforms.push_back("gl_FragDepth");

		mShaderEditor = new TextEditor();
		mShaderEditor->setFontScale(1.5f);
		mShaderEditor->SetLanguageDefinition(TextEditor::LanguageDefinition::GLSL_TINY(
			{
				"vec2", "vec3", "vec4"
				, "mat3", "mat4"
				, "sampler2D", "samplerCube"
				, "uniform", "layout", "in", "out", "location"
			},
			uniforms));
	}

	MyShaderEditorWindow::~MyShaderEditorWindow()
	{
		sShaderEditorRegister.erase(mFilePath);
		delete mShaderEditor;
	}

	void MyShaderEditorWindow::loadFile(std::filesystem::path& file)
	{
		mFilePath = std::move(file);
		mName = mFilePath.filename().string();
		mIsShader = mFilePath.extension().string() == ".tysl" ? true : false;

		auto tex = FileTool::loadText(mFilePath.string());
		mShaderEditor->SetText(tex);
	}

	bool MyShaderEditorWindow::begin()
	{
		return ImGui::Begin(mName.c_str()
			, &mIsOpen
			, ImGuiWindowFlags_NoScrollbar
			| ImGuiWindowFlags_MenuBar);
	}

	MyShaderEditorWindow* MyShaderEditorWindow::create(std::filesystem::path& file, GUI* host)
	{
		auto result = sShaderEditorRegister.try_emplace(file, nullptr);
		if (result.second)
		{
			auto editor = new MyShaderEditorWindow();
			editor->loadFile(file);
			editor->open(host);
			result.first->second = editor;
		}

		return result.first->second;
	}

	MyShaderEditorWindow* MyShaderEditorWindow::create(GUI* host)
	{
		auto editor = new MyShaderEditorWindow();
		editor->open(host);
		return editor;
	}

	void MyShaderEditorWindow::onRender()
	{
		GUIWindow::onRender();

		auto cpos = mShaderEditor->GetCursorPosition();
		ImGui::SetWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("编辑(Edit)"))
			{
				bool ro = mShaderEditor->IsReadOnly();

				if (ImGui::MenuItem("只读(Read-only mode)", nullptr, &ro))
				{
					mShaderEditor->SetReadOnly(ro);
				}
				ImGui::Separator();

				if (ImGui::MenuItem("撤销(Undo)", "ALT-Backspace", nullptr, !ro && mShaderEditor->CanUndo()))
				{
					mShaderEditor->Undo();
				}
				if (ImGui::MenuItem("重做(Redo)", "Ctrl-Y", nullptr, !ro && mShaderEditor->CanRedo()))
				{
					mShaderEditor->Redo();
				}

				ImGui::Separator();

				if (ImGui::MenuItem("复制(Copy)", "Ctrl-C", nullptr, mShaderEditor->HasSelection()))
				{
					mShaderEditor->Copy();
				}

				if (ImGui::MenuItem("剪切(Cut)", "Ctrl-X", nullptr, !ro && mShaderEditor->HasSelection()))
				{
					mShaderEditor->Cut();
				}

				if (ImGui::MenuItem("粘贴(Paste)", "Ctrl-V", nullptr, !ro && ImGui::GetClipboardText() != nullptr))
				{
					mShaderEditor->Paste();
				}

				if (ImGui::MenuItem("清空(Delete)", "Del", nullptr, !ro && mShaderEditor->HasSelection()))
				{
					mShaderEditor->Delete();
				}


				ImGui::Separator();

				if (ImGui::MenuItem("全选(Select all)", nullptr, nullptr))
				{
					mShaderEditor->SetSelection(TextEditor::Coordinates(), TextEditor::Coordinates(mShaderEditor->GetTotalLines(), 0));
				}

				ImGui::EndMenu();
			}

			if (mIsShader)
			{
				if (ImGui::MenuItem("编译&保存(Rebuild&Save)"))
				{
					auto textToSave = mShaderEditor->GetText();
					FileTool::saveFile(mFilePath.string(), textToSave);

					std::regex rg_find_name(R"(#TINY_CFG_BEGIN[\s\S]*\{[\s\S]*str\s*Name\s*=\s*(.*);[\s\S]*\}[\s\S]*#TINY_CFG_END)");
					std::sregex_iterator it(textToSave.begin(), textToSave.end(), rg_find_name);
					std::string name = (*it)[1];

					ShaderManager::rebuild(name, textToSave);
				}
			}
			else
			{
				if (ImGui::MenuItem("保存(Save)"))
				{
					auto textToSave = mShaderEditor->GetText();
					FileTool::saveFile(mFilePath.string(), textToSave);
				}
			}

			auto value = mShaderEditor->getFontScale();
			ImGui::SetNextItemWidth(60);
			ImGui::DragFloat("字体缩放(FontScale)", &value, 0.1f, 0.1f, 10.0f, "%.1f");
			mShaderEditor->setFontScale(value);

			/*
			if (ImGui::BeginMenu("View"))
			{
				if (ImGui::MenuItem("Dark palette"))
				{
					mShaderEditor->SetPalette(TextEditor::GetDarkPalette());
				}
				if (ImGui::MenuItem("Light palette"))
				{
					mShaderEditor->SetPalette(TextEditor::GetLightPalette());
				}
				if (ImGui::MenuItem("Retro blue palette"))
				{
					mShaderEditor->SetPalette(TextEditor::GetRetroBluePalette());
				}
				ImGui::EndMenu();
			}
			*/
			ImGui::EndMenuBar();
		}

		ImGui::Text("%6d/%-6d %6d 行(lines)  | %s | %s | %s | %s"
		, cpos.mLine + 1, cpos.mColumn + 1
		, mShaderEditor->GetTotalLines()
		, mShaderEditor->IsOverwrite() ? "Ovr" : "Ins"
		, mShaderEditor->CanUndo() ? "*" : "X"
		, mShaderEditor->GetLanguageDefinition().mName.c_str()
		, mFilePath.string().c_str());

		ImGui::Separator();

		mShaderEditor->Render("TextEditor");

		//ImGui::GetFont()->FontSize = font_size;
	}
}

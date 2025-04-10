#pragma once
#include "../EditorContext.h"

namespace tezcat::Editor
{
	class EditorShaderEditorWindow : public GUIWindow
	{
		struct ShaderFileData
		{
			Shader* mSelected = nullptr;
		};

		EditorShaderEditorWindow();
	public:
		virtual ~EditorShaderEditorWindow();

		void loadFile(std::filesystem::path& file);
		bool begin() override;

		static EditorShaderEditorWindow* create(std::filesystem::path& file, GUI* host);
		static EditorShaderEditorWindow* create(GUI* host);

	protected:
		void onRender() override;

	private:
		std::unique_ptr<TextEditor> mShaderEditor;
		std::filesystem::path mFilePath;
		bool mIsShader;

		ShaderFileData mShaderFileData;

		static std::unordered_map<std::filesystem::path, EditorShaderEditorWindow*> sShaderEditorRegister;
	};

}

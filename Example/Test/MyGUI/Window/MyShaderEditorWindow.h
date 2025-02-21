#pragma once
#include "../MyGUIContext.h"

namespace tezcat::Editor
{
	class MyShaderEditorWindow : public GUIWindow
	{
		struct ShaderFileData
		{
			Shader* mSelected = nullptr;
		};

		MyShaderEditorWindow();
	public:
		virtual ~MyShaderEditorWindow();

		void loadFile(std::filesystem::path& file);
		bool begin() override;

		static MyShaderEditorWindow* create(std::filesystem::path& file, GUI* host);
		static MyShaderEditorWindow* create(GUI* host);

	protected:
		void onRender() override;

	private:
		std::unique_ptr<TextEditor> mShaderEditor;
		std::filesystem::path mFilePath;
		bool mIsShader;

		ShaderFileData mShaderFileData;

		static std::unordered_map<std::filesystem::path, MyShaderEditorWindow*> sShaderEditorRegister;
	};

}

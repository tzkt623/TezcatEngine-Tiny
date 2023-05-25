#pragma once
#include "../MyGUIContext.h"


class MyShaderEditorWindow : public GUIWindow
{
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
	TextEditor* mShaderEditor;
	std::filesystem::path mFilePath;
	bool mIsShader;

	static std::unordered_map<std::filesystem::path, MyShaderEditorWindow*> sShaderEditorRegister;
};



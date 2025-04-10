#pragma once

#include "../EditorContext.h"

namespace tezcat::Editor
{
	class EditorTextureViewerWindow : public GUIWindow
	{
	public:
		EditorTextureViewerWindow(const std::string& name);
		virtual ~EditorTextureViewerWindow();

		void setTexture(Texture* tex);
		void loadTexture(const std::filesystem::path& path);

		bool begin() override;
	protected:

		void onRender() override;
	private:
		Texture* mTex;
		ImVec2 mSize;
	};

}

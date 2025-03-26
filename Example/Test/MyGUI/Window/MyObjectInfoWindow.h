#pragma once

#include "../MyGUIContext.h"

namespace tezcat::Editor
{
	class MyObjectInfoWindow : public GUIWindow
	{
		struct DrawMaterial
		{
			bool flag;
			MeshRenderer* mr;
		};

		TINY_EDITOR_WINDOW_INSTANCE_H(MyObjectInfoWindow)

	protected:
		void onRender() override;
		int resetName(ImGuiInputTextCallbackData* data);


	private:
		void drawDefault(Component* com) {}

	private:
		template<typename Com>
		void drawComponent(std::function<void(Component*)>&& function)
		{
			auto id = Com::getComponentTypeIDStatic();
			if (mDrawFunctions.size() <= id)
			{
				mDrawFunctions.resize(id + 1, TINY_BIND_THIS(MyObjectInfoWindow::drawDefault));
			}

			mDrawFunctions[id] = std::move(function);
		}

		void drawExtraMaterial();

	private:
		std::string mNameBuffer;
		//TinyWeakRef<GameObject> mGameObject;
		std::vector<std::function<void(Component*)>> mDrawFunctions;
		MeshRenderer* mMeshRenderer;
	};

}

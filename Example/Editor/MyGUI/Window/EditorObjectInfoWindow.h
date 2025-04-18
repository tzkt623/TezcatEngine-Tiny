#pragma once

#include "../EditorContext.h"

namespace tezcat::Editor
{
	class EditorObjectInfoWindow : public GUIWindow
	{
		struct DrawMaterial
		{
			bool flag;
			MeshRenderer* mr;
		};

		TINY_EDITOR_WINDOW_INSTANCE_H(EditorObjectInfoWindow)

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
				mDrawFunctions.resize(id + 1, TINY_BIND_THIS(EditorObjectInfoWindow::drawDefault));
			}

			mDrawFunctions[id] = std::move(function);
		}

		void drawExtraMaterial();

	private:
		std::vector<std::function<void(Component*)>> mDrawFunctions;
		std::vector<std::function<void(Uniform*, UniformValueConfig*, const int32_t&)>> mShaderValueFunctionArray;
		MeshRenderer* mMeshRenderer;
	};

}

#pragma once

#include "../Head/CppHead.h"

namespace tezcat::Tiny::Core
{
	class GameObject;
	class Camera;
	class IRenderObejct;
	class Layer
	{
	public:
		Layer();
		~Layer();

		void addRenderObejct(IRenderObejct* gameObject);
		void cullGameObject(Camera* camera);

	private:
		int m_Index;
		std::list<ref<IRenderObejct>> m_RenderObejctList;
	};
}
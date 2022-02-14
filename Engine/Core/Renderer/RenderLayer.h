#pragma once

#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"

namespace tezcat::Tiny::Core
{
	class Camera;
	class IRenderObejct;
	class TINY_API RenderLayer
	{
	public:
		RenderLayer();
		~RenderLayer();
	public:
		void addRenderObejct(IRenderObejct* gameObject);
		void cullGameObject(Camera* camera);

	private:
		int m_Index;
		std::list<ref<IRenderObejct>> m_RenderObjectList;
	};
}
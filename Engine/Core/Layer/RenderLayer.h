#pragma once

#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"
#include "../Renderer/RenderConfig.h"

namespace tezcat::Tiny::Core
{
	class IRenderObserver;
	class IRenderObject;
	class TINY_API RenderLayer
	{
	public:
		RenderLayer();
		~RenderLayer();
	public:
		void addRenderObejct(IRenderObject* gameObject);
		void culling(IRenderObserver* renderObserver, const RenderPassType& passType);

	private:
		int mIndex;
		std::list<IRenderObject*> mRenderObjectList;
		std::list<IRenderObject*> mVisibleRenderObjectList;

	public:
		static RenderLayer* getRenderLayer(uint32_t index)
		{
			return sLayerAry[index];
		}

		static void addRenderObejct(uint32_t index, IRenderObject* renderObject)
		{
			sLayerAry[index]->addRenderObejct(renderObject);
		}

	private:

		static std::array<RenderLayer*, 32> sLayerAry;
	};
}

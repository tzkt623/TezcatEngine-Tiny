#pragma once

#include "../Head/TinyCpp.h"
#include "../Head/ConfigHead.h"
#include "RenderConfig.h"
#include "RenderAgent.h"

namespace tezcat::Tiny
{
	class IRenderObserver;
	class RenderAgent;

	//-------------------------------------------------------
	//
	//	RenderLayer
	//
	class TINY_API RenderLayer
	{
	public:
		RenderLayer();
		~RenderLayer();
	public:
		void culling(IRenderObserver* renderObserver, const RenderPassType& passType);
		void addRenderAgent(RenderAgent* renderAgent);

	private:
		int mIndex;
		std::list<TinyWeakRef<RenderAgent>> mRenderObjectList;

	public:
		static RenderLayer* getRenderLayer(uint32_t index)
		{
			return sLayerAry[index];
		}

		static void addRenderAgent(uint32_t index, RenderAgent* renderAgent)
		{
			sLayerAry[index]->addRenderAgent(renderAgent);
		}

		static void init();

	private:
		static std::array<RenderLayer*, 32> sLayerAry;
	};
}

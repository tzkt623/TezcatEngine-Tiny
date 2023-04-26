#pragma once

#include "../Head/TinyCpp.h"
#include "../Head/ConfigHead.h"
#include "../Renderer/RenderConfig.h"
#include "../Renderer/RenderAgent.h"

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


	private:
		static std::array<RenderLayer*, 32> sLayerAry;
	};
}

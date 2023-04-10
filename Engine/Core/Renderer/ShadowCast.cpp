#include "ShadowCast.h"
#include "../Renderer/RenderPass.h"



namespace tezcat::Tiny::Core
{
	ShadowCast::ShadowCast()
		: mDirty(true)
		, mRenderPassAry()
		, mRenderPassUMap()
	{

	}

	ShadowCast::~ShadowCast()
	{

	}

	void ShadowCast::init()
	{
	}

	void ShadowCast::addPass(RenderPass* pass)
	{
		mRenderPassAry.push_back(pass);
		mDirty = true;
	}

	void ShadowCast::render(BaseGraphics* graphics, IRenderObserver* renderObject)
	{
		if (mDirty)
		{
			mDirty = false;
			std::sort(mRenderPassAry.begin(), mRenderPassAry.end(), [](RenderPass* a, RenderPass* b)
			{
				return a->getOrderID() < b->getOrderID();
			});
		}

		for (auto pass : mRenderPassAry)
		{
			pass->render(graphics, renderObject);
		}
	}

	void ShadowCast::render(BaseGraphics* graphics, Camera* camera)
	{

	}
}

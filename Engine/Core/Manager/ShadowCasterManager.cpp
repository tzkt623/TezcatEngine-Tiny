#include "ShadowCasterManager.h"
#include "ShaderManager.h"
#include "TextureManager.h"

#include "../Shader/ShaderPackage.h"
#include "../Shader/Shader.h"

#include "../Component/ShadowCaster.h"
#include "../Renderer/RenderLayer.h"
#include "../Renderer/BaseGraphics.h"
#include "../Event/EngineEvent.h"

namespace tezcat::Tiny
{
	ShadowCasterManager::ShadowCasterManager()
		: mShader(nullptr)
		, mShadowTexture(nullptr)
	{
		ShadowCasterMgr::attach(this);
		EngineEvent::get()->addListener(EngineEventID::EE_OnPopScene, this, [this](const EventData& data)
		{
			mCasterAry.clear();
		});
	}

	Shader* ShadowCasterManager::getShader()
	{
		if (mShader == nullptr)
		{
			mShader = ShaderMgr::getInstance()->find("Unlit/ShadowMap");
		}

		return mShader;
	}

	void ShadowCasterManager::calculate(BaseGraphics* graphics)
	{
		/*
		* 预渲染阶段
		* 1.渲染阴影贴图
		*  a.每一个可以产生阴影的灯都要渲染一张
		*/
		auto it = mCasterAry.begin();
		auto end = mCasterAry.end();
		while (it != end)
		{
			if (auto prt = (*it).lock())
			{
				//先剔除
				auto& cull_list = prt->getCullLayerList();
				for (auto& index : cull_list)
				{
					auto queue = prt->getRenderQueue();
					graphics->addPreRenderPassQueue((ExtraQueue*)queue);
					//把能看见的对象全部剔除到阴影通道
					RenderLayer::getRenderLayer(index)->culling(graphics, prt, queue);
				}

				++it;
			}
			else
			{
				it = mCasterAry.erase(it);
			}
		}
	}


	uint32_t ShadowCasterManager::addCaster(ShadowCaster* caster)
	{
		mCasterAry.push_back(caster);
		return mCasterAry.size() - 1;
	}

	void ShadowCasterManager::submit(BaseGraphics* graphics, Shader* shader)
	{
		if (mCasterAry.empty())
		{
			return;
		}

		//这里可以用于处理所有带阴影的灯光
		//目前这里暂时这么写
		for (auto& caster : mCasterAry)
		{
			caster->submit(graphics, shader);
			graphics->setGlobalTexture2D(shader, ShaderParam::TexDepth, caster->getShadwowTexutre());
		}
	}
}


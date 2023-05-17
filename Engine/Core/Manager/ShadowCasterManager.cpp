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
			mShader = ShaderMgr::getInstance()
				->findPackage("Unlit/ShadowMap")
				->getShaders()[0];
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
		for (auto caster : mCasterAry)
		{
			if (caster == nullptr)
			{
				continue;
			}

			//先剔除
			auto& cull_list = caster->getCullLayerList();
			for (auto& index : cull_list)
			{
				auto queue = caster->getRenderQueue();
				graphics->addPreRenderPassQueue((ExtraQueue*)queue);
				//把能看见的对象全部剔除到阴影通道
				RenderLayer::getRenderLayer(index)->culling(caster, queue);
			}
		}
	}

	void ShadowCasterManager::removeCaster(ShadowCaster* caster)
	{
		mFreeIDs.push_back(caster->getCasterID());
		mCasterAry[caster->getCasterID()] = nullptr;
	}

	uint32_t ShadowCasterManager::addCaster(ShadowCaster* caster)
	{
		if (mFreeIDs.empty())
		{
			uint32_t index = (uint32_t)mCasterAry.size();
			mCasterAry.push_back(caster);
			return index;
		}
		else
		{
			uint32_t index = (uint32_t)mFreeIDs.front();
			mFreeIDs.pop_front();
			mCasterAry[index] = caster;
			return index;
		}
	}

	void ShadowCasterManager::submit(Shader* shader)
	{
		if (!mShadowTexture)
		{
			mShadowTexture = (Texture2D*)TextureMgr::getInstance()->findTexture("Shadow");
		}

		//这里可以用于处理所有带阴影的灯光
		//目前这里暂时这么写
		for (auto caster : mCasterAry)
		{
			if (caster != nullptr)
			{
				caster->submitViewMatrix(shader);
				shader->setGlobalTexture2D(ShaderParam::TexDepth, mShadowTexture);
				break;
			}
		}
	}

}


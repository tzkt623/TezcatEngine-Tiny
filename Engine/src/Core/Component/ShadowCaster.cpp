/*
	Copyright (C) 2024 Tezcat(特兹卡特) tzkt623@qq.com

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "Core/Component/ShadowCaster.h"
#include "Core/Component/Transform.h"
#include "Core/Component/GameObject.h"
#include "Core/Component/Light.h"

#include "Core/Shader/ShaderParam.h"
#include "Core/Shader/Shader.h"

#include "Core/Manager/FrameBufferManager.h"
#include "Core/Manager/TextureManager.h"
#include "Core/Manager/ShaderManager.h"

#include "Core/Renderer/Texture.h"
#include "Core/Renderer/FrameBuffer.h"
#include "Core/Renderer/BaseGraphics.h"
#include "Core/Renderer/Pipeline.h"
#include "Core/Renderer/Renderer.h"
#include "Core/Renderer/RenderCommand.h"

#include "Core/Manager/ShadowCasterManager.h"
#include "Core/Renderer/RenderObserver.h"


namespace tezcat::Tiny
{
	TINY_OBJECT_CPP(ShadowCaster, ComponentAutoID<ShadowCaster>);

	ShadowCaster::ShadowCaster()
		: mShadowObserver()
		, mShadowTexture(nullptr)
		, mPipePass(nullptr)
		, mFrameBuffer(nullptr)
		, mUID(0)
	{

	}

	ShadowCaster::~ShadowCaster()
	{

	}

	void ShadowCaster::init()
	{
		Base::init();

		mShadowObserver = ShadowObserver::create();
		mShadowObserver->saveObject();
		mShadowObserver->setCullLayer(0);
		mShadowObserver->setSortingID(2);

		mPipePass = ReplacedPipelinePass::create(mShadowObserver
			, ShaderManager::find("Hide/ShadowMap"));
		//mPipePass->setAutoCulling([](BaseMeshRenderer* renderer)
		//	{
		//		return new RenderCMD_DrawMeshWithOutMaterial(renderer->getVertex(), renderer->getTransform());
		//	});
		mPipePass->saveObject();
	}

	void ShadowCaster::onStart()
	{
		mShadowObserver->setTransform(mGameObject->getTransform());

		mLightType = mGameObject->getComponent<LightComponent>()->getLightType();
		switch (mLightType)
		{
		case LightType::Directional:
			mShadowTexture = ShadowCasterManager::getOrCreateDirectionalShadowMap();
			break;
		case LightType::Point:
			mShadowTexture = ShadowCasterManager::getOrCreatePointShadowMap();
			break;
		case LightType::Spot:
			mShadowTexture = ShadowCasterManager::getOrCreateSpotShadowMap();
			break;
		default:
			break;
		}

		mFrameBuffer = FrameBuffer::create(std::format("{}{}", mGameObject->getName(), mUID));
		mFrameBuffer->saveObject();
		mFrameBuffer->addAttachment(mShadowTexture);
		mFrameBuffer->generate();

		auto [w, h, l] = mShadowTexture->getSizeWHL();
		mShadowObserver->setFrameBuffer(mFrameBuffer);
		mShadowObserver->setViewRect(0, 0, w, h);
	}

	void ShadowCaster::onEnable()
	{
		mUID = ShadowCasterManager::add(this);
		mPipePass->addToPipeline();
	}

	void ShadowCaster::onDisable()
	{
		ShadowCasterManager::recycle(this);
		mPipePass->removeFromPipeline();
		mUID = 0;
	}

	void ShadowCaster::onDestroy()
	{
		switch (mLightType)
		{
		case LightType::Directional:
			ShadowCasterManager::recycleDirectionalShadowMap((Texture2D*)mFrameBuffer->getAttachment(0));
			break;
		case LightType::Point:
			ShadowCasterManager::recyclePointShadowMap((TextureCube*)mFrameBuffer->getAttachment(0));
			break;
		case LightType::Spot:
			ShadowCasterManager::recycleSpotShadowMap((Texture2D*)mFrameBuffer->getAttachment(0));
			break;
		default:
			break;
		}

		ShadowCasterManager::recycle(this);
		mUID = 0;

		mFrameBuffer->deleteObject();
		mShadowObserver->deleteObject();
		mPipePass->deleteObject();
		mShadowTexture = nullptr;
	}

	void ShadowCaster::setShadowMap(int width, int height, const std::string& shaderName)
	{
		if (mFrameBuffer)
		{
			return;
		}
	}

	void ShadowCaster::submit(Shader* shader)
	{
		auto pv = mShadowObserver->getProjectionMatrix() * mShadowObserver->getViewMatrix();
		Graphics::getInstance()->setMat4(shader, ShaderParam::MatrixLightVP, pv);

		switch (mLightType)
		{
		case LightType::Directional:
			Graphics::getInstance()->setGlobalTexture2D(shader, ShaderParam::TexShadow, (Texture2D*)mShadowTexture);
			break;
		case LightType::Point:
			break;
		case LightType::Spot:
			break;
		default:
			break;
		}
	}
}

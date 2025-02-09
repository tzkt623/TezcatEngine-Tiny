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

#include "Core/Shader/ShaderParam.h"
#include "Core/Shader/Shader.h"

#include "Core/Manager/FrameBufferManager.h"
#include "Core/Manager/TextureManager.h"
#include "Core/Manager/ShaderManager.h"

#include "Core/Renderer/Texture.h"
#include "Core/Renderer/FrameBuffer.h"
#include "Core/Renderer/BaseGraphics.h"
#include "Core/Renderer/Pipeline.h"

#include "Core/Manager/ShadowCasterManager.h"

namespace tezcat::Tiny
{
	TINY_OBJECT_CPP(ShadowCaster, ComponentT<ShadowCaster>)

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

		mShadowObserver = RenderObserver::create();
		mShadowObserver->saveObject();
		mShadowObserver->setOrderID(-126);

		mPipePass = ReplacedPipelinePass::create(mShadowObserver
			, ShaderManager::find("Unlit/ShadowMap"));
		mPipePass->setUseCullLayerData(true);
		mPipePass->saveObject();
	}

	void ShadowCaster::onStart()
	{
		mShadowObserver->setTransform(mGameObject->getTransform());
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
		ShadowCasterManager::recycle(this);
		mUID = 0;

		mFrameBuffer->deleteObject();
		mShadowObserver->deleteObject();
		mShadowTexture->deleteObject();
		mPipePass->deleteObject();
	}

	void ShadowCaster::setShadowMap(int width, int height, const std::string& shaderName)
	{
		if (mFrameBuffer)
		{
			return;
		}

		mShadowTexture = Texture2D::create("Shadow");
		mShadowTexture->saveObject();
		mShadowTexture->setConfig(width, height
			, TextureInternalFormat::Depth
			, TextureFormat::Depth
			, DataMemFormat::Float);
		mShadowTexture->setAttachPosition(TextureAttachPosition::DepthComponent);

		mFrameBuffer = FrameBuffer::create(shaderName + std::to_string(mUID));
		mFrameBuffer->saveObject();
		mFrameBuffer->addAttachment(mShadowTexture);
		mFrameBuffer->generate();

		mShadowObserver->setFrameBuffer(mFrameBuffer);
		mShadowObserver->setViewRect(0, 0, width, height);
	}

	void ShadowCaster::submit(Shader* shader)
	{
		auto pv = mShadowObserver->getProjectionMatrix() * mShadowObserver->getViewMatrix();
		Graphics::getInstance()->setMat4(shader, ShaderParam::MatrixLightVP, pv);
		Graphics::getInstance()->setGlobalTexture2D(shader, ShaderParam::TexDepth, mShadowTexture);
	}

}

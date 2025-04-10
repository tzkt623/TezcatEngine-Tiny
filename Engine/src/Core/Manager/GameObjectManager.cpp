/*
	Copyright (C) 2025 Tezcat(特兹卡特) tzkt623@qq.com

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

#include "Core/Manager/GameObjectManager.h"
#include "Core/Manager/FrameBufferManager.h"
#include "Core/Manager/TextureManager.h"
#include "Core/Manager/ShaderManager.h"
#include "Core/Manager/CameraManager.h"

#include "Core/Component/GameObject.h"
#include "Core/Component/Camera.h"

#include "Core/Renderer/Texture.h"
#include "Core/Renderer/FrameBuffer.h"
#include "Core/Renderer/Pipeline.h"
#include "Core/Renderer/RenderCommand.h"
#include "Core/Renderer/Renderer.h"
#include "Core/Renderer/RenderObserver.h"

#include "Core/Engine.h"

namespace tezcat::Tiny
{
	std::queue<int32_t> GameObjectManager::mFreeIDQueue;
	std::vector<GameObject*> GameObjectManager::mArray;
	ReplacedPipelinePass* GameObjectManager::mPass = nullptr;
	FrameBuffer* GameObjectManager::mFrameBuffer = nullptr;
	RenderObserver* GameObjectManager::mObserver = nullptr;;

	int32_t GameObjectManager::addGameObject(GameObject* gameObject)
	{
		gameObject->saveObject();
		if (mFreeIDQueue.empty())
		{
			int32_t id = (int32_t)mArray.size();
			mArray.push_back(gameObject);
			return id;
		}
		else
		{
			int32_t id = mFreeIDQueue.front();
			mFreeIDQueue.pop();
			TINY_ASSERT(mArray[id] == nullptr);
			mArray[id] = gameObject;
			return id;
		}
	}

	void GameObjectManager::removeGameObject(GameObject* gameObject)
	{
		int32_t id = gameObject->getUID();
		mFreeIDQueue.push(id);
		mArray[id] = nullptr;
	}

	void GameObjectManager::init()
	{
		//第一个Object默认为空
		mArray.push_back(nullptr);
	}

	void GameObjectManager::active()
	{
	}

	void GameObjectManager::deactive()
	{
		mPass->removeFromPipeline();
	}

	void GameObjectManager::preRender()
	{
		if (!CameraManager::isDataValied())
		{
			return;
		}

		if (mPass == nullptr)
		{
			if (CameraManager::isDataValied())
			{
				if (mFrameBuffer == nullptr)
				{
					createFrameBuffer();
				}

				if (mObserver)
				{
					createPass(mObserver);
					mPass->addToPipeline();
				}
			}
		}
	}

	void GameObjectManager::enterScene()
	{

	}

	void GameObjectManager::exitScene()
	{
		//if (mPass)
		//{
		//	mPass->removeFromPipeline();
		//	mPass->deleteObject();
		//}
		//
		//mPass = nullptr;
		//mObserver = nullptr;
	}

	void GameObjectManager::setIDObserver(RenderObserver* observer)
	{
 		if (mObserver != observer)
		{
			if (mPass)
			{
				mPass->removeFromPipeline();
				mPass->deleteObject();
			}

			mObserver = observer;
			mPass = nullptr;
		}
	}

	void GameObjectManager::createPass(BaseRenderObserver* observer)
	{
		mPass = ReplacedPipelinePass::create(observer, ShaderManager::find("Unlit/ObjectID"));
		mPass->setFrameBuffer(mFrameBuffer);
		mPass->setAutoCulling([](BaseMeshRenderer* renderer)
			{
				return new RenderCMD_DrawID(renderer->getVertex()
					, renderer->getTransform());
			});
		mPass->setUserSortingID(1);
		mPass->saveObject();
	}

	void GameObjectManager::createFrameBuffer()
	{
		auto [flag1, frame_buffer] = FrameBufferManager::create("FB_ObjectID");
		if (flag1 == FlagCreate::Succeeded)
		{
			auto [flag2, texture] = TextureManager::create2D("CB_TexObjectID");
			if (flag2 == FlagCreate::Succeeded)
			{
				texture->setSize(Engine::getScreenWidth(), Engine::getScreenHeight());
				texture->setFormat(TextureInternalFormat::R32UI, TextureFormat::RI);
				texture->setDataMemFormat(DataMemFormat::UInt32);

				texture->setFilter(TextureFilter::Linear, TextureFilter::Linear);
				texture->setWrap(TextureWrap::Clamp_To_Edge, TextureWrap::Clamp_To_Edge);
				texture->setAttachPosition(TextureAttachPosition::ColorComponent);
			}

			auto [flag3, texture_viewer] = TextureManager::create2D("CB_TexObjectIDViewer");
			if (flag3 == FlagCreate::Succeeded)
			{
				texture_viewer->setSize(Engine::getScreenWidth(), Engine::getScreenHeight());
				texture_viewer->setFormat(TextureInternalFormat::RGBA8, TextureFormat::RGBA);
				texture_viewer->setDataMemFormat(DataMemFormat::UByte);

				texture_viewer->setFilter(TextureFilter::Linear, TextureFilter::Linear);
				texture_viewer->setWrap(TextureWrap::Clamp_To_Edge, TextureWrap::Clamp_To_Edge);
				texture_viewer->setAttachPosition(TextureAttachPosition::ColorComponent);
			}

			auto [flag4, render2d] = TextureManager::createRender2D("RB_TexObjectIDDepth");
			if (flag4 == FlagCreate::Succeeded)
			{
				render2d->setConfig(Engine::getScreenWidth(), Engine::getScreenHeight(), TextureInternalFormat::Depth);
				render2d->setAttachPosition(TextureAttachPosition::DepthComponent);
			}

			frame_buffer->addAttachment(texture);
			frame_buffer->addAttachment(texture_viewer);
			frame_buffer->addAttachment(render2d);
			frame_buffer->generate();

			mFrameBuffer = frame_buffer;
		}
	}

	bool GameObjectManager::allowPickObject()
	{
		return mPass != nullptr;
	}

}

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

#include "Core/Renderer/Renderer.h"
#include "Core/Renderer/FrameBuffer.h"
#include "Core/Renderer/Pipeline.h"
#include "Core/Renderer/PipelineWorker.h"
#include "Core/Renderer/RenderObserver.h"
#include "Core/Renderer/BaseGraphics.h"
#include "Core/Renderer/RenderCommand.h"

#include "Core/Component/GameObject.h"

#include "Core/Manager/VertexBufferManager.h"
#include "Core/Manager/PipelineManager.h"

#include "Core/Data/MeshData.h"


namespace tezcat::Tiny
{
	TINY_OBJECT_CPP(BaseRenderer, TinyObject);

	//---------------------------------------------------------------
	//
	// IRenderMesh
	//
	TINY_OBJECT_CPP(BaseMeshRenderer, BaseRenderer);
	BaseMeshRenderer::BaseMeshRenderer()
		: mMaterial(nullptr)
		, mVertex(nullptr)
		, mIsShadowReciever(true)
		, mTransform(nullptr)
		, mLayerID(nullptr)
	{

	}

	BaseMeshRenderer::~BaseMeshRenderer()
	{
		mVertex->deleteObject();
		mMaterial->deleteObject();
		mTransform = nullptr;
		mLayerID = nullptr;
	}

	void BaseMeshRenderer::setMaterial(Material* material)
	{
		if (mMaterial)
		{
			mMaterial->deleteObject();
		}

		mMaterial = material;
		mMaterial->saveObject();
	}

	void BaseMeshRenderer::setMesh(const std::string& meshName)
	{
		if (mVertex)
		{
			mVertex->deleteObject();
		}

		mVertex = VertexBufferManager::create(meshName);
		mVertex->saveObject();
	}

	void BaseMeshRenderer::setMesh(MeshData* meshData)
	{
		if (mVertex)
		{
			mVertex->deleteObject();
		}

		mVertex = VertexBufferManager::create(meshData);
		mVertex->saveObject();
	}

	void BaseMeshRenderer::setMesh(Vertex* vertex)
	{
		if (mVertex)
		{
			mVertex->deleteObject();
		}

		mVertex = vertex;
		mVertex->saveObject();
	}

	void BaseMeshRenderer::makeRenderCommand(PipelineQueue* queue)
	{
		//auto pass = PipelineManager::createOrGetObserverPass(mMaterial->getShader(), renderObserver);

		auto pass = queue->createOrGetObserverPass(mMaterial->getShader());
		pass->addCommand<RenderCMD_DrawMeshWithMaterial>(mVertex, mTransform, mMaterial);

		//auto pass = renderObserver->createOrGetPass(mMaterial->getShader());
		//auto pass = PipelinePassManager::createOrGetPass(*mLayerID, mMaterial->getShader());
		//pass->addCommand<RenderCMD_DrawMeshWithMaterial>(mVertex, mTransform, mMaterial);
	}

	void BaseMeshRenderer::makeRenderCommand(ReplacedPipelinePass* pass)
	{
		pass->pushCommand(this);
		//		pass->addCommand<RenderCMD_DrawMeshWithOutMaterial>(mVertex, mTransform);
	}

	void BaseMeshRenderer::setLayer(GameObject* go)
	{
		mLayerID = &(go->mLayerMaskIndex);
	}
}

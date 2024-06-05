#include "Renderer.h"
#include "FrameBuffer.h"
#include "Pipeline.h"
#include "RenderObserver.h"
#include "BaseGraphics.h"

#include "../Manager/VertexBufferManager.h"
#include "../Data/MeshData.h"


namespace tezcat::Tiny
{
	TINY_OBJECT_CPP(BaseRenderer, TinyObject)

	//---------------------------------------------------------------
	//
	// IRenderMesh
	//
	TINY_OBJECT_CPP(BaseMeshRenderer, BaseRenderer)
	BaseMeshRenderer::BaseMeshRenderer()
		: mMaterial(nullptr)
		, mVertex(nullptr)
		, mIsShadowReciever(true)
		, mTransform(nullptr)
	{

	}

	BaseMeshRenderer::~BaseMeshRenderer()
	{
		mVertex->deleteObject();
		mMaterial->deleteObject();
		mTransform = nullptr;
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

	void BaseMeshRenderer::submit(Shader* shader)
	{
		mMaterial->submit(shader);
	}

	void BaseMeshRenderer::makeRenderCommand(BaseRenderObserver* renderObserver)
	{
		auto pass = renderObserver->createOrGetPass(mMaterial->getShader());
		auto cmd = Graphics::getInstance()->createDrawMeshCMD(mVertex, mTransform, mMaterial);

		//new RenderCommand(mVertex, mTransform, mMaterial);

		pass->addCommand(cmd);
	}

	void BaseMeshRenderer::makeRenderCommand(PipelinePass* pass)
	{
		pass->addCommand(Graphics::getInstance()->createDrawShadowCMD(mVertex, mTransform));
	}
}

#include "RenderObject.h"
#include "FrameBuffer.h"
#include "RenderPass.h"
#include "../Manager/VertexBufferManager.h"
#include "../Data/MeshData.h"


namespace tezcat::Tiny
{
	//---------------------------------------------------------------
	//
	// IRenderMesh
	//
	IRenderMesh::IRenderMesh()
		: mRenderAgent(RenderAgent::create(this))
		, mMaterial(nullptr)
		, mVertex(nullptr)
		, mIsShadowReciever(true)
	{
		mRenderAgent->addRef();
	}

	IRenderMesh::~IRenderMesh()
	{
		mVertex->subRef();
		mRenderAgent->subRef();
		mMaterial->subRef();
	}

	void IRenderMesh::setMaterial(Material* material)
	{
		if (mMaterial)
		{
			mMaterial->subRef();
		}

		mMaterial = material;
		mMaterial->addRef();
	}

	void IRenderMesh::setMesh(const std::string& meshName)
	{
		if (mVertex)
		{
			mVertex->subRef();
		}

		mVertex = VertexBufMgr::getInstance()->create(meshName);
		mVertex->addRef();
		mRenderAgent->setVertex(mVertex);
	}

	void IRenderMesh::setMesh(MeshData* meshData)
	{
		if (mVertex)
		{
			mVertex->subRef();
		}

		mVertex = VertexBufMgr::getInstance()->create(meshData);
		mVertex->addRef();
		mRenderAgent->setVertex(mVertex);
	}

	void IRenderMesh::setMesh(Vertex* vertex)
	{
		if (mVertex)
		{
			mVertex->subRef();
		}

		mVertex = vertex;
		mVertex->addRef();
	}

	void IRenderMesh::submit(BaseGraphics* graphics, Shader* shader)
	{
		mMaterial->submit(graphics, shader);
	}

	//---------------------------------------------------------------
	//
	// IRenderObserver
	//
	IRenderObserver::IRenderObserver(RenderQueue* queue)
		: mCullMask(0)
		, mFrameBuffer(FrameBuffer::getDefaultBuffer())
		, mNearFace(0.1f)
		, mFarFace(100.0f)
		, mFOV(60.0f)
		, mProjectionMatrix(1.0f)
		, mViewType(ViewType::Screen)
		, mOrder(0)
		, mDirty(true)
		, mRenderPhase(RenderPhase::Forward)
		, mRenderQueue(queue)
	{
		this->setCullLayer(0);
		mFrameBuffer->addRef();
	}


	IRenderObserver::~IRenderObserver()
	{
		if (mFrameBuffer)
		{
			mFrameBuffer->subRef();
		}

		delete mRenderQueue;
	}

	void IRenderObserver::setOrtho(float near, float far)
	{
		mViewType = ViewType::Ortho;
		mNearFace = near;
		mFarFace = far;
		mDirty = true;
	}

	void IRenderObserver::setPerspective(float fov, float near, float far)
	{
		mViewType = ViewType::Perspective;
		mFOV = fov;
		mNearFace = near;
		mFarFace = far;
		mDirty = true;
	}

	void IRenderObserver::setViewRect(int x, int y, int width, int height)
	{
		mViewInfo.OX = x;
		mViewInfo.OY = y;
		mViewInfo.Width = width;
		mViewInfo.Height = height;
		mDirty = true;
	}

	void IRenderObserver::updateObserverMatrix()
	{
		if (mDirty)
		{
			mDirty = false;
			switch (mViewType)
			{
			case ViewType::Ortho:
			{
				auto hw = mViewInfo.Width / 2.0f;
				auto hh = mViewInfo.Height / 2.0f;
				mProjectionMatrix = glm::ortho(
					mViewInfo.OX - hw, mViewInfo.OX + hw,
					mViewInfo.OY - hh, mViewInfo.OY + hh,
					mNearFace, mFarFace);
				break;
			}
			case ViewType::Perspective:
			{
				mProjectionMatrix = glm::perspective(
					glm::radians(mFOV),
					(float)mViewInfo.Width / (float)mViewInfo.Height,
					mNearFace, mFarFace);
				break;
			}
			default:
				break;
			}
		}
	}

	void IRenderObserver::setFrameBuffer(FrameBuffer* buffer)
	{
		if (mFrameBuffer)
		{
			mFrameBuffer->subRef();
		}

		if (buffer)
		{
			mFrameBuffer = buffer;
		}
		else
		{
			mFrameBuffer = FrameBuffer::getDefaultBuffer();
		}

		mFrameBuffer->addRef();
	}

}

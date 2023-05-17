#include "RenderObject.h"
#include "FrameBuffer.h"
#include "RenderPass.h"
#include "../Manager/BufferManager.h"


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

		mVertex = BufferMgr::getInstance()->getVertex(meshName);
		mVertex->addRef();
		mRenderAgent->setVertex(mVertex);
	}

	void IRenderMesh::setMesh(MeshData* meshData)
	{
		if (mVertex)
		{
			mVertex->subRef();
		}

		mVertex = BufferMgr::getInstance()->getVertex(meshData);
		mVertex->addRef();
		mRenderAgent->setVertex(mVertex);
	}

	void IRenderMesh::submit(Shader* shader)
	{
		mMaterial->submit(shader);
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
		, mViewType(ViewType::Perspective)
		, mOrder(0)
		, mPMatDirty(true)
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
		mPMatDirty = true;
	}

	void IRenderObserver::setPerspective(float fov, float near, float far)
	{
		mViewType = ViewType::Perspective;
		mFOV = fov;
		mNearFace = near;
		mFarFace = far;
		mPMatDirty = true;
	}

	void IRenderObserver::setViewRect(int x, int y, int width, int height)
	{
		mViewInfo.OX = x;
		mViewInfo.OY = y;
		mViewInfo.Width = width;
		mViewInfo.Height = height;
		mPMatDirty = true;
	}

	void IRenderObserver::updateObserverMatrix()
	{
		if (mPMatDirty)
		{
			mPMatDirty = false;
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

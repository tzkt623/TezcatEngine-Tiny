#include "RenderObject.h"
#include "FrameBuffer.h"

namespace tezcat::Tiny
{
	IRenderObserver::IRenderObserver()
		: mCullMask(0)
		, mFrameBuffer(FrameBuffer::getDefaultBuffer())
		, mNearFace(0.1f)
		, mFarFace(100.0f)
		, mFOV(60.0f)
		, mProjectionMatrix(1.0f)
		, mType(Type::Perspective)
		, mPMatDirty(true)
	{
		this->setCullLayer(0);
		mFrameBuffer->addRef();
	}


	void IRenderObserver::setOrtho(float near, float far)
	{
		mType = Type::Ortho;
		mNearFace = near;
		mFarFace = far;
		mPMatDirty = true;
	}

	void IRenderObserver::setPerspective(float fov, float near, float far)
	{
		mType = Type::Perspective;
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
			switch (mType)
			{
			case Type::Ortho:
			{
				auto hw = mViewInfo.Width / 2.0f;
				auto hh = mViewInfo.Height / 2.0f;
				mProjectionMatrix = glm::ortho(
					mViewInfo.OX - hw, mViewInfo.OX + hw,
					mViewInfo.OY - hh, mViewInfo.OY + hh,
					mNearFace, mFarFace);
				break;
			}
			case Type::Perspective:
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

	IRenderObserver::~IRenderObserver()
	{
		if (mFrameBuffer)
		{
			mFrameBuffer->subRef();
		}
	}
}

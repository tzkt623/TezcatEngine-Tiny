#include "FrameBuffer.h"
#include "Texture.h"
#include "BaseGraphics.h"
#include "Pipeline.h"

#include "../Manager/FrameBufferManager.h"


namespace tezcat::Tiny
{
	TINY_OBJECT_CPP(FrameBuffer, TinyObject)

	FrameBuffer::FrameBuffer()
		: mBufferID(0)
		, mComponents()
		, mName()
		, mUID(FrameBufferManager::giveID())
		, mCurrentFrame(Pipeline::getFrameCount() - 1)
	{

	}

	FrameBuffer::FrameBuffer(std::string name)
		: mBufferID(0)
		, mComponents()
		, mName(std::move(name))
		, mUID(FrameBufferManager::giveID())
		, mCurrentFrame(Pipeline::getFrameCount() - 1)
	{

	}

	FrameBuffer::~FrameBuffer()
	{
		FrameBufferManager::recycleID(mUID);
	}

	void FrameBuffer::generate()
	{
		Graphics::getInstance()->cmdCreateFrameBuffer(this);
	}

	void FrameBuffer::addAttachment(Texture* tex)
	{
		tex->saveObject();
		mComponents.push_back(tex);
	}
}

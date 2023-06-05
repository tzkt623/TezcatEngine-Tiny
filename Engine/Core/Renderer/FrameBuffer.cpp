#include "FrameBuffer.h"
#include "../Renderer/Texture.h"
#include "../Renderer/BaseGraphics.h"


namespace tezcat::Tiny
{
	FrameBuffer* FrameBuffer::sDefaultBuffer = new FrameBuffer();
	TinyStack<FrameBuffer*> FrameBuffer::sFrameBufferStack;

	TINY_RTTI_CPP(FrameBuffer);

	FrameBuffer::FrameBuffer()
		: mBufferID(0)
		, mComponents()
		, mName()
	{

	}

	FrameBuffer::FrameBuffer(const std::string& name)
		: mBufferID(0)
		, mComponents()
		, mName(name)
	{

	}

	FrameBuffer::~FrameBuffer()
	{

	}

	Texture* FrameBuffer::getBuffer(const int& index)
	{
		return mComponents[index];
	}

	void FrameBuffer::bind(BaseGraphics* graphics, FrameBuffer* buffer)
	{
		buffer->addRef();
		sFrameBufferStack.push(buffer);
		graphics->bind(buffer);
	}

	void FrameBuffer::unbind(BaseGraphics* graphics, FrameBuffer* buffer)
	{
		if (sFrameBufferStack.top() != buffer)
		{
			throw std::invalid_argument("Unbind FrameBuffer Error!!! the buffer must be the same");
		}

		sFrameBufferStack.top()->subRef();

		if (sFrameBufferStack.empty())
		{
			return;
		}

		sFrameBufferStack.pop();
		graphics->bind(sFrameBufferStack.top());
	}

	void FrameBuffer::generate()
	{
		Graphics::getInstance()->cmdCreateFrameBuffer(this);
	}

	void FrameBuffer::addAttachment(Texture* tex)
	{
		mComponents.push_back(tex);
		tex->addRef();
	}
}

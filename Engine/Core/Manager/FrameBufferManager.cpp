#include "FrameBufferManager.h"
#include "TextureManager.h"
#include "../Engine.h"

#include "../Renderer/Texture.h"
#include "../Renderer/FrameBuffer.h"
#include "../Renderer/BaseGraphics.h"

namespace tezcat::Tiny
{
	FrameBuffer* FrameBufferManager::sDefaultBuffer = nullptr;
	uint32 FrameBufferManager::sID = 0;
	std::queue<uint32> FrameBufferManager::sFreeIDs;
	std::stack<FrameBuffer*> FrameBufferManager::sFrameBufferStack;
	std::unordered_map<std::string_view, FrameBuffer*> FrameBufferManager::sUMap;
	FrameBuffer* FrameBufferManager::sMainBuffer = nullptr;

	void FrameBufferManager::init()
	{
		sDefaultBuffer = FrameBuffer::create("Tiny_Default_FrameBuffer");
		sDefaultBuffer->saveObject();
		sFrameBufferStack.push(sDefaultBuffer);
	}

	std::tuple<bool, FrameBuffer*> FrameBufferManager::create(std::string name)
	{
		auto result = sUMap.find(name);
		if (result != sUMap.end())
		{
			return { false, result->second };
		}

		auto frame_buffer = FrameBuffer::create();
		frame_buffer->setName(name);
		frame_buffer->saveObject();
		sUMap.emplace(frame_buffer->getName(), frame_buffer);
		return { true, frame_buffer };
	}

	void FrameBufferManager::bind(FrameBuffer* buffer)
	{
		buffer->saveObject();
		sFrameBufferStack.push(buffer);
		Graphics::getInstance()->bind(buffer);
	}

	void FrameBufferManager::unbind(FrameBuffer* buffer)
	{
		if (sFrameBufferStack.top() != buffer)
		{
			throw std::invalid_argument("Unbind FrameBuffer Error!!! the buffer must be the same");
		}

		sFrameBufferStack.top()->deleteObject();

		if (sFrameBufferStack.empty())
		{
			return;
		}

		sFrameBufferStack.pop();
		Graphics::getInstance()->bind(sFrameBufferStack.top());
	}

	FrameBuffer* FrameBufferManager::getMainFrameBufferBuildin()
	{
		if (!sMainBuffer)
		{
			auto [flag1, frame_buffer] = create(TINY_FRAMEBUFFER_VIEWPORT);
			if (flag1)
			{
				auto [flag2, tex_color] = TextureManager::create2D(TINY_FRAMEBUFFER_VIEWPORT_COLOR_TEXTURE);
				if (flag2)
				{
					tex_color->setConfig(Engine::getScreenWidth(), Engine::getScreenHeight()
						, TextureInternalFormat::RGBA
						, TextureFormat::RGBA);
					tex_color->setAttachPosition(TextureAttachPosition::ColorComponent);
					frame_buffer->addAttachment(tex_color);
				}

				auto [flag3, tex_depth] = TextureManager::create2D(TINY_FRAMEBUFFER_VIEWPORT_DEPTH_TEXTURE);
				if (flag3)
				{
					tex_depth->setConfig(Engine::getScreenWidth(), Engine::getScreenHeight()
						, TextureInternalFormat::Depth
						, TextureFormat::Depth);
					tex_depth->setAttachPosition(TextureAttachPosition::DepthComponent);
					frame_buffer->addAttachment(tex_depth);
				}

				frame_buffer->generate();
				sMainBuffer = frame_buffer;
			}
		}

		return sMainBuffer;
	}
}


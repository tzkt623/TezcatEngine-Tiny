#pragma once

#include "../Head/TinyCpp.h"
#include "../Head/ConfigHead.h"
#include "../Base/TinyObject.h"

namespace tezcat::Tiny
{
	class Texture;
	/*
	* TODO: Attachment所属权的问题
	*/
	class TINY_API FrameBuffer : public TinyObject
	{
		friend class FrameBufferManager;

		FrameBuffer();
		FrameBuffer(std::string name);
		TINY_OBJECT_H(FrameBuffer, TinyObject)
	public:
		virtual ~FrameBuffer();
		void generate();

	public:
		void apply(uint32 id) { mBufferID = id; }
		uint32 getFrameBufferID() const { return mBufferID; }
		void addAttachment(Texture* tex);
		Texture* getAttachment(int index) { return mComponents[index]; }

		const std::vector<Texture*>& getAttachmentes() const { return mComponents; }
		std::string_view getNameView() const { return mName; }
		std::string& getName() { return mName; }
		void setName(std::string& name) { mName.assign(std::move(name)); }

		uint32 currentFrame() const { return mCurrentFrame; }
		void updateCurrentFrame(uint32 val) { mCurrentFrame = val; }

	protected:
		uint32 mCurrentFrame;
		uint32 mBufferID;
		uint32 mUID;
		std::string mName;
		std::vector<Texture*> mComponents;
	};
}


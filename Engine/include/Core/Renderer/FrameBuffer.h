#pragma once

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
		std::string getMemoryInfo() override;

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
		void onClose() override;

	protected:
		uint32 mCurrentFrame;
		uint32 mBufferID;
		uint32 mUID;
		std::string mName;
		std::vector<Texture*> mComponents;
	};
}


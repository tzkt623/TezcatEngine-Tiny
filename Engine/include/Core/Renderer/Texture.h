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
#include "../Renderer/RenderConfig.h"
#include "../Head/ConfigHead.h"
#include "../Base/TinyObject.h"

namespace tezcat::Tiny
{
	class Image;
	class Shader;

	class TINY_API TextureAgent
	{
		virtual void bind(Shader* shader) = 0;
	};

	class TINY_API Texture2DAgent : public TextureAgent
	{

	};

	class TINY_API Texture3DAgent : public TextureAgent
	{

	};

	class TINY_API TextureCubeAgent : public TextureAgent
	{

	};

	class TINY_API TextureRender2DAgent : public TextureAgent
	{

	};

	class TINY_API Texture : public TinyObject
	{
		TINY_ABSTRACT_OBJECT_H(Texture, TinyObject)

	protected:
		Texture(const TextureFormat& internalFormat
			  , const TextureFormat& format
			  , const TextureFilter& minFilter
			  , const TextureFilter& magFilter
			  , const DataMemFormat& dataType);

	public:

		virtual ~Texture();

		virtual void generate() = 0;
		virtual void update() {};
		virtual TextureType getTextureType() const = 0;

		virtual std::tuple<uint32, uint32, uint32> getSizeWHL() = 0;

		const uint32& getTextureID() const { return mTextureID; }
		virtual void apply(uint32 id) { mTextureID = id; }

		const uint32 getUID() const { return mUID; }
		const std::string_view getNameView() const { return mName; }
		const std::string& getName() const { return mName; }
		void setName(std::string& name) { mName.assign(std::move(name)); }
		void setName(const std::string& name) { mName = name; }

	public:
		const DataMemFormatWrapper& getDataMemFormat() const { return mDataMemFormat; }
		void setDataMemFormat(const DataMemFormat& memFormat)
		{
			mDataMemFormat = ContextMap::DataMemFormatArray[(uint32)memFormat];
		}

		const TextureAttachPosition& getAttachPosition() const { return mAttachPosition; }
		void setAttachPosition(const TextureAttachPosition& val) { mAttachPosition = val; }

	public:
		const TexFilterWrapper& getMinFilter() const { return mMinFilter; }
		const TexFilterWrapper& getMagFilter() const { return mMagFilter; }

		void setMinFilter(const TextureFilter& filter)
		{
			mMinFilter = ContextMap::TextureFilterArray[(uint32)filter];
		}

		void setMagFilter(const TextureFilter& filter)
		{
			mMagFilter = ContextMap::TextureFilterArray[(uint32)filter];
		}

		void setFilter(const TextureFilter& min, const TextureFilter& mag)
		{
			mMinFilter = ContextMap::TextureFilterArray[(uint32)min];
			mMagFilter = ContextMap::TextureFilterArray[(uint32)mag];
		}

	public:
		const TexInternalFormatWrapper& getInternalFormat() const { return mInternalFormat; }
		const TexFormatWrapper& getFormat() const { return mFormat; }

		void setTexFormat(const TextureFormat& channel)
		{
			mFormat = ContextMap::TextureFormatArray[(uint32)channel];
		}

		void setInternalFormat(const TextureInternalFormat& channel)
		{
			mInternalFormat = ContextMap::TextureInternalFormatArray[(uint32)channel];
		}

		void setFormat(const TextureInternalFormat& internalChannel, const TextureFormat& channel)
		{
			mFormat = ContextMap::TextureFormatArray[(uint32)channel];
			mInternalFormat = ContextMap::TextureInternalFormatArray[(uint32)internalChannel];
		}

	public:
		static TexFormatWrapper getTextureFormat(const Image& image);
		static TexInternalFormatWrapper getTextureInternalFormat(const Image& image);

	protected:
		std::string mName;
		uint32 mUID;
		uint32 mTextureID;
		TextureAttachPosition mAttachPosition;
		TexFilterWrapper mMinFilter;
		TexFilterWrapper mMagFilter;
		TexInternalFormatWrapper mInternalFormat;
		TexFormatWrapper mFormat;
		DataMemFormatWrapper mDataMemFormat;

	private:
		static uint32 sUIDGiver;
		static std::deque<uint32> sFreeUIDs;
		static uint32 giveUID();
	};

	class TINY_API Texture2D : public Texture
	{
		Texture2D();
		Texture2D(std::string name);
		TINY_OBJECT_H(Texture2D, Texture)
	public:
		virtual ~Texture2D();

		TextureType getTextureType() const override { return TextureType::Texture2D; }

		virtual void setImage(const Image* image);
		virtual void updateData(const Image* image);

		void setConfig(const uint32& width, const uint32& height
			, const TextureInternalFormat& internalFormat, const TextureFormat& format
			, const DataMemFormat& dataType = DataMemFormat::UByte
			, const TextureFilter& min = TextureFilter::Linear
			, const TextureFilter& mag = TextureFilter::Linear
			, const TextureWrap& wrapS = TextureWrap::Clamp_To_Edge
			, const TextureWrap& wrapT = TextureWrap::Clamp_To_Edge);

		void generate() override;
		void update() override;

		std::tuple<uint32, uint32, uint32> getSizeWHL() override
		{
			return { mWidth, mHeight, 0 };
		}

		void setSize(const int& width, const int& height)
		{
			mWidth = width;
			mHeight = height;
		}

		void apply(uint32 id);

		uint32 getWidth() const { return mWidth; }
		uint32 getHeight() const { return mHeight; }
		void* getData() const { return mData; }

		bool isHDR() const { return mIsHDR; }

		const TexWrapWrapper& getWrapS() const { return mWrapS; }
		const TexWrapWrapper& getWrapT() const { return mWrapT; }

	protected:
		uint32 mWidth;
		uint32 mHeight;
		TexWrapWrapper mWrapS;
		TexWrapWrapper mWrapT;
		bool mIsHDR;
		void* mData;
	};

	class TINY_API Texture3D : public Texture
	{
		Texture3D();
		TINY_OBJECT_H(Texture3D, Texture)

	public:
		virtual ~Texture3D();

		TextureType getTextureType() const final { return TextureType::Texture3D; }

		void setConfig(const uint32& width, const uint32& hegiht, const uint32& length
			, const TextureInternalFormat& internalFormat, const TextureFormat& format
			, const DataMemFormat& dataType
			, const TextureFilter& min = TextureFilter::Linear
			, const TextureFilter& mag = TextureFilter::Linear
			, const TextureWrap& wrapS = TextureWrap::Clamp_To_Edge
			, const TextureWrap& wrapT = TextureWrap::Clamp_To_Edge
			, const TextureWrap& wrapR = TextureWrap::Clamp_To_Edge);

		uint32 getWidth() const { return mWidth; }
		uint32 getHeight() const { return mHeight; }
		uint32 getLength() const { return mLength; }

		virtual std::tuple<uint32, uint32, uint32> getSizeWHL() final override
		{
			return { mWidth, mHeight, mLength };
		}


	protected:
		uint32 mWidth;
		uint32 mHeight;
		uint32 mLength;
		TexWrapWrapper mWrapS;
		TexWrapWrapper mWrapT;
		TexWrapWrapper mWrapR;
	};

	class TINY_API TextureCube : public Texture
	{
		TextureCube();
		TextureCube(std::string name);
		TINY_OBJECT_H(TextureCube, Texture)

	public:
		virtual ~TextureCube();

		TextureType getTextureType() const final { return TextureType::TextureCube; }

		virtual void setImage(const std::array<Image*, 6>& images);

		void setConfig(const uint32& size
			, const TextureInternalFormat& internalFormat, const TextureFormat& format
			, const DataMemFormat& dataType
			, const TextureFilter& min = TextureFilter::Linear
			, const TextureFilter& mag = TextureFilter::Linear
			, const TextureWrap& wrapS = TextureWrap::Clamp_To_Edge
			, const TextureWrap& wrapT = TextureWrap::Clamp_To_Edge
			, const TextureWrap& wrapR = TextureWrap::Clamp_To_Edge);

		virtual void generate();

		const TexWrapWrapper& getWrapS() const { return mWrapS; }
		const TexWrapWrapper& getWrapT() const { return mWrapT; }
		const TexWrapWrapper& getWrapR() const { return mWrapR; }

		void setSize(uint32 size) { mSize = size; }
		uint32 getSize() const { return mSize; }

		void* getData(uint32 index) const { return mDatas[index]; }
		void apply(uint32 id);

		virtual std::tuple<uint32, uint32, uint32> getSizeWHL() final override { return { mSize, mSize, mSize }; }

	protected:
		uint32 mSize;
		TexWrapWrapper mWrapS;
		TexWrapWrapper mWrapT;
		TexWrapWrapper mWrapR;

		void* mDatas[6];
	};

	/// <summary>
	/// WriteOnlyBuffer
	/// Used By FrameBuffer
	/// </summary>
	class TINY_API TextureRender2D : public Texture
	{
		TextureRender2D();
		TextureRender2D(std::string name);
		TINY_OBJECT_H(TextureRender2D, Texture)
	public:
		virtual ~TextureRender2D();

		TextureType getTextureType() const override { return TextureType::TextureRender2D; }
		void generate() override;

		void setConfig(const uint32& width, const uint32& height, const TextureInternalFormat& internalFormat);

		uint32 getWidth() const { return mWidth; }
		uint32 getHeight() const { return mHeight; }

		const TexWrapWrapper& getWrapS() const { return mWrapS; }
		const TexWrapWrapper& getWrapT() const { return mWrapT; }

		virtual std::tuple<uint32, uint32, uint32> getSizeWHL() final override
		{
			return { mWidth, mHeight, 0 };
		}

	protected:
		uint32 mWidth;
		uint32 mHeight;
		TexWrapWrapper mWrapS;
		TexWrapWrapper mWrapT;
	};
}

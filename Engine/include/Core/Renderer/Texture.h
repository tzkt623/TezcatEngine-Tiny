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

		virtual std::tuple<uint32_t, uint32_t, uint32_t> getSizeWHL() = 0;

		const uint32_t& getTextureID() const { return mTextureID; }
		virtual void apply(uint32_t id) { mTextureID = id; }

		const uint32_t getUID() const { return mUID; }
		const std::string_view getNameView() const { return mName; }
		const std::string& getName() const { return mName; }
		void setName(std::string& name)
		{
			mName.assign(std::move(name));
			mEngineName = mName;
		}
		void setName(const std::string& name)
		{
			mName = name;
		}
		std::string getMemoryInfo() override;

	public:
		const DataMemFormatWrapper& getDataMemFormat() const { return mDataMemFormat; }
		void setDataMemFormat(const DataMemFormat& memFormat)
		{
			mDataMemFormat = ContextMap::DataMemFormatArray[(uint32_t)memFormat];
		}

		const TextureAttachPosition& getAttachPosition() const { return mAttachPosition; }
		void setAttachPosition(const TextureAttachPosition& val) { mAttachPosition = val; }

	public:
		const TexFilterWrapper& getMinFilter() const { return mMinFilter; }
		const TexFilterWrapper& getMagFilter() const { return mMagFilter; }

		void setMinFilter(const TextureFilter& filter)
		{
			mMinFilter = ContextMap::TextureFilterArray[(uint32_t)filter];
		}

		void setMagFilter(const TextureFilter& filter)
		{
			mMagFilter = ContextMap::TextureFilterArray[(uint32_t)filter];
		}

		void setFilter(const TextureFilter& min, const TextureFilter& mag)
		{
			mMinFilter = ContextMap::TextureFilterArray[(uint32_t)min];
			mMagFilter = ContextMap::TextureFilterArray[(uint32_t)mag];
		}

	public:
		const TexInternalFormatWrapper& getInternalFormat() const { return mInternalFormat; }
		const TexFormatWrapper& getFormat() const { return mFormat; }

		void setTexFormat(const TextureFormat& channel)
		{
			mFormat = ContextMap::TextureFormatArray[(uint32_t)channel];
		}

		void setInternalFormat(const TextureInternalFormat& channel)
		{
			mInternalFormat = ContextMap::TextureInternalFormatArray[(uint32_t)channel];
		}

		void setFormat(const TextureInternalFormat& internalChannel, const TextureFormat& channel)
		{
			mInternalFormat = ContextMap::TextureInternalFormatArray[(uint32_t)internalChannel];
			mFormat = ContextMap::TextureFormatArray[(uint32_t)channel];
		}

	public:
		static TexFormatWrapper getTextureFormat(const Image& image);
		static TexInternalFormatWrapper getTextureInternalFormat(const Image& image);

	protected:
		std::string mName;
		uint32_t mUID;
		uint32_t mTextureID;
		TextureAttachPosition mAttachPosition;
		TexFilterWrapper mMinFilter;
		TexFilterWrapper mMagFilter;
		TexInternalFormatWrapper mInternalFormat;
		TexFormatWrapper mFormat;
		DataMemFormatWrapper mDataMemFormat;

	private:
		static uint32_t sUIDGiver;
		static std::deque<uint32_t> sFreeUIDs;
		static uint32_t giveUID();
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

		void setConfig(const uint32_t& width, const uint32_t& height
			, const TextureInternalFormat& internalFormat, const TextureFormat& format
			, const DataMemFormat& dataType = DataMemFormat::UByte
			, const TextureFilter& min = TextureFilter::Linear
			, const TextureFilter& mag = TextureFilter::Linear
			, const TextureWrap& wrapS = TextureWrap::Clamp_To_Edge
			, const TextureWrap& wrapT = TextureWrap::Clamp_To_Edge);

		void generate() override;
		void update() override;

		std::tuple<uint32_t, uint32_t, uint32_t> getSizeWHL() override
		{
			return { mWidth, mHeight, 0 };
		}

		void setSize(const int& width, const int& height)
		{
			mWidth = width;
			mHeight = height;
		}

		void apply(uint32_t id);

		uint32_t getWidth() const { return mWidth; }
		uint32_t getHeight() const { return mHeight; }
		void* getData() const { return mData; }

		bool isHDR() const { return mIsHDR; }

		const TexWrapWrapper& getWrapS() const { return mWrapS; }
		const TexWrapWrapper& getWrapT() const { return mWrapT; }

		void setWrap(const TextureWrap& S, const TextureWrap& T)
		{
			mWrapS = ContextMap::TextureWrapArray[(uint32_t)S];
			mWrapT = ContextMap::TextureWrapArray[(uint32_t)T];
		}

	protected:
		uint32_t mWidth;
		uint32_t mHeight;
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

		void setConfig(const uint32_t& width, const uint32_t& hegiht, const uint32_t& length
			, const TextureInternalFormat& internalFormat, const TextureFormat& format
			, const DataMemFormat& dataType
			, const TextureFilter& min = TextureFilter::Linear
			, const TextureFilter& mag = TextureFilter::Linear
			, const TextureWrap& wrapS = TextureWrap::Clamp_To_Edge
			, const TextureWrap& wrapT = TextureWrap::Clamp_To_Edge
			, const TextureWrap& wrapR = TextureWrap::Clamp_To_Edge);

		uint32_t getWidth() const { return mWidth; }
		uint32_t getHeight() const { return mHeight; }
		uint32_t getLength() const { return mLength; }

		virtual std::tuple<uint32_t, uint32_t, uint32_t> getSizeWHL() final override
		{
			return { mWidth, mHeight, mLength };
		}


	protected:
		uint32_t mWidth;
		uint32_t mHeight;
		uint32_t mLength;
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

		void setConfig(const uint32_t& size
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

		void setSize(uint32_t size) { mSize = size; }
		uint32_t getSize() const { return mSize; }

		void* getData(uint32_t index) const { return mDatas[index]; }
		void apply(uint32_t id);

		virtual std::tuple<uint32_t, uint32_t, uint32_t> getSizeWHL() final override { return { mSize, mSize, mSize }; }

	protected:
		uint32_t mSize;
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

		void setConfig(const uint32_t& width, const uint32_t& height, const TextureInternalFormat& internalFormat);

		uint32_t getWidth() const { return mWidth; }
		uint32_t getHeight() const { return mHeight; }

		const TexWrapWrapper& getWrapS() const { return mWrapS; }
		const TexWrapWrapper& getWrapT() const { return mWrapT; }

		virtual std::tuple<uint32_t, uint32_t, uint32_t> getSizeWHL() final override
		{
			return { mWidth, mHeight, 0 };
		}

	protected:
		uint32_t mWidth;
		uint32_t mHeight;
		TexWrapWrapper mWrapS;
		TexWrapWrapper mWrapT;
	};
}

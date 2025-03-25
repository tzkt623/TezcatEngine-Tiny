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

#include "Core/Renderer/Texture.h"
#include "Core/Renderer/BaseGraphics.h"
#include "Core/Renderer/RenderCommand.h"

#include "Core/Manager/TextureManager.h"
#include "Core/Data/Image.h"


namespace tezcat::Tiny
{
	uint32_t Texture::sUIDGiver = 0;
	std::deque<uint32_t> Texture::sFreeUIDs;

	TINY_OBJECT_CPP(Texture, TinyObject);
	Texture::Texture(const TextureFormat& internalFormat
					   , const TextureFormat& format
					   , const TextureFilter& minFilter
					   , const TextureFilter& magFilter
					   , const DataMemFormat& dataType)
		: mUID(giveUID())
		, mTextureID(0)
		, mAttachPosition(TextureAttachPosition::None)
		, mMinFilter(ContextMap::TextureFilterArray[(uint32_t)minFilter])
		, mMagFilter(ContextMap::TextureFilterArray[(uint32_t)magFilter])
		, mInternalFormat(ContextMap::TextureInternalFormatArray[(uint32_t)internalFormat])
		, mFormat(ContextMap::TextureFormatArray[(uint32_t)format])
		, mDataMemFormat(ContextMap::DataMemFormatArray[(uint32_t)dataType])
		, mName()
	{

	}

	Texture::~Texture()
	{
		sFreeUIDs.push_back(mUID);
	}

	TexFormatWrapper Texture::getTextureFormat(const Image& image)
	{
		switch (image.getChannels())
		{
		case 1: return ContextMap::TextureFormatArray[(uint32_t)TextureFormat::R];
		case 2: return ContextMap::TextureFormatArray[(uint32_t)TextureFormat::RG];
		case 3: return ContextMap::TextureFormatArray[(uint32_t)TextureFormat::RGB];
		case 4: return ContextMap::TextureFormatArray[(uint32_t)TextureFormat::RGBA];
		default:
			break;
		}

		return ContextMap::TextureFormatArray[(uint32_t)TextureFormat::None];
	}

	uint32_t Texture::giveUID()
	{
		if (sFreeUIDs.empty())
		{
			return sUIDGiver++;
		}

		auto uid = sFreeUIDs.front();
		sFreeUIDs.pop_front();
		return uid;
	}

	TexInternalFormatWrapper Texture::getTextureInternalFormat(const Image& image)
	{
		switch (image.getChannels())
		{
		case 1: return image.isHDR() ? ContextMap::TextureInternalFormatArray[(uint32_t)TextureInternalFormat::R16F] : ContextMap::TextureInternalFormatArray[(uint32_t)TextureInternalFormat::R];
		case 2: return image.isHDR() ? ContextMap::TextureInternalFormatArray[(uint32_t)TextureInternalFormat::RG16F] : ContextMap::TextureInternalFormatArray[(uint32_t)TextureInternalFormat::RG];
		case 3: return image.isHDR() ? ContextMap::TextureInternalFormatArray[(uint32_t)TextureInternalFormat::RGB16F] : ContextMap::TextureInternalFormatArray[(uint32_t)TextureInternalFormat::RGB];
		case 4: return image.isHDR() ? ContextMap::TextureInternalFormatArray[(uint32_t)TextureInternalFormat::RGBA16F] : ContextMap::TextureInternalFormatArray[(uint32_t)TextureInternalFormat::RGBA];
		default:
			break;
		}

		return ContextMap::TextureInternalFormatArray[(uint32_t)TextureInternalFormat::None];
	}

	std::string Texture::getMemoryInfo()
	{
		return TINY_OBJECT_MEMORY_INFO();
	}

	//------------------------------------------------------------
	//
	//	Texture2D
	//
	TINY_OBJECT_CPP(Texture2D, Texture);
	Texture2D::Texture2D()
		: Base(TextureFormat::RGBA
			, TextureFormat::RGBA
			, TextureFilter::Linear
			, TextureFilter::Linear
			, DataMemFormat::UByte)
		, mWidth(0)
		, mHeight(0)
		, mIsHDR(false)
		, mData(nullptr)
	{

	}

	Texture2D::Texture2D(std::string name)
		: Texture2D()
	{
		mName.assign(std::move(name));
		mEngineName = mName;
	}

	Texture2D::~Texture2D()
	{
		Graphics::getInstance()->addCommand(new RenderCMD_DeleteTexture2D(this));
	}

	void Texture2D::setImage(const Image* image)
	{
		mWidth = image->getWidth();
		mHeight = image->getHeight();
		mIsHDR = image->isHDR();

		uint64 size = image->getDataSize();
		auto temp = realloc(mData, size);
		if (temp)
		{
			mData = temp;
			memcpy(mData, image->getData(), size);
		}
		else
		{
			throw std::bad_exception();
		}

		mAttachPosition = TextureAttachPosition::ColorComponent;
		mWrapS = ContextMap::TextureWrapArray[(uint32_t)TextureWrap::Clamp_To_Edge];
		mWrapT = ContextMap::TextureWrapArray[(uint32_t)TextureWrap::Clamp_To_Edge];
		mMinFilter = ContextMap::TextureFilterArray[(uint32_t)TextureFilter::Linear];
		mMagFilter = ContextMap::TextureFilterArray[(uint32_t)TextureFilter::Linear];
		mInternalFormat = getTextureInternalFormat(*image);
		mFormat = getTextureFormat(*image);
		mDataMemFormat = ContextMap::DataMemFormatArray[(uint32_t)(image->isHDR() ? DataMemFormat::Float : DataMemFormat::UByte)];
	}

	void Texture2D::setConfig(const uint32_t& width, const uint32_t& height
		, const TextureInternalFormat& internalFormat, const TextureFormat& format
		, const DataMemFormat& dataType
		, const TextureFilter& min /*= TextureFilter::Linear */
		, const TextureFilter& mag /*= TextureFilter::Linear */
		, const TextureWrap& wrapS /*= TextureWrap::Clamp_To_Edge */
		, const TextureWrap& wrapT /*= TextureWrap::Clamp_To_Edge*/)
	{
		mWidth = width;
		mHeight = height;

		mInternalFormat = ContextMap::TextureInternalFormatArray[(uint32_t)internalFormat];
		mFormat = ContextMap::TextureFormatArray[(uint32_t)format];
		mDataMemFormat = ContextMap::DataMemFormatArray[(uint32_t)dataType];

		mWrapS = ContextMap::TextureWrapArray[(uint32_t)wrapS];
		mWrapT = ContextMap::TextureWrapArray[(uint32_t)wrapT];
		mMinFilter = ContextMap::TextureFilterArray[(uint32_t)min];
		mMagFilter = ContextMap::TextureFilterArray[(uint32_t)mag];
	}

	void Texture2D::updateData(const Image* image)
	{
		mWidth = image->getWidth();
		mHeight = image->getHeight();
		mIsHDR = image->isHDR();

		mInternalFormat = getTextureInternalFormat(*image);
		mFormat = getTextureFormat(*image);
		mDataMemFormat = ContextMap::DataMemFormatArray[(uint32_t)(image->isHDR() ? DataMemFormat::Float : DataMemFormat::UByte)];

		free(mData);

		uint64 size = image->getDataSize();
		auto temp = realloc(mData, size);
		if (temp)
		{
			mData = temp;
			memcpy(mData, image->getData(), size);
		}
	}

	void Texture2D::generate()
	{
		if (mTextureID != 0)
		{
			//Graphics::getInstance()->cmdDeleteTexture2D(mTextureID);
			Graphics::getInstance()->addCommand(new RenderCMD_DeleteTexture2D(this));
		}

		Graphics::getInstance()->addCommand(new RenderCMD_CreateTexture2D(this));
	}

	void Texture2D::update()
	{
		Graphics::getInstance()->cmdUpdateTexture2D(this);
	}

	void Texture2D::apply(uint32_t id)
	{
		Base::apply(id);
		free(mData);
		mData = nullptr;
	}

	void Texture2D::clearInGPU()
	{
		Graphics::getInstance()->addCommand<RenderCMD_ClearTexture2D>(this);
	}

	//--------------------------------------------------------
	//
	//	Texture3D
	//
	TINY_OBJECT_CPP(Texture3D, Texture);
	Texture3D::~Texture3D()
	{

	}

	void Texture3D::setConfig(const uint32_t& width, const uint32_t& hegiht, const uint32_t& length
		, const TextureInternalFormat& internalFormat, const TextureFormat& format
		, const DataMemFormat& dataType
		, const TextureFilter& min /*= TextureFilter::Linear */
		, const TextureFilter& mag /*= TextureFilter::Linear */
		, const TextureWrap& wrapS /*= TextureWrap::Clamp_To_Edge */
		, const TextureWrap& wrapT /*= TextureWrap::Clamp_To_Edge */
		, const TextureWrap& wrapR /*= TextureWrap::Clamp_To_Edge*/)
	{
		mWidth = width;
		mHeight = hegiht;
		mLength = length;

		mInternalFormat = ContextMap::TextureInternalFormatArray[(uint32_t)internalFormat];
		mFormat = ContextMap::TextureFormatArray[(uint32_t)format];
		mDataMemFormat = ContextMap::DataMemFormatArray[(uint32_t)dataType];

		mMinFilter = ContextMap::TextureFilterArray[(uint32_t)min];
		mMagFilter = ContextMap::TextureFilterArray[(uint32_t)mag];

		mWrapS = ContextMap::TextureWrapArray[(uint32_t)wrapS];
		mWrapT = ContextMap::TextureWrapArray[(uint32_t)wrapT];
		mWrapR = ContextMap::TextureWrapArray[(uint32_t)wrapR];
	}

	//--------------------------------------------------------
	//
	//	TextureCubes
	//
	TINY_OBJECT_CPP(TextureCube, Texture);
	TextureCube::TextureCube()
		: Base(TextureFormat::RGBA
			, TextureFormat::RGBA
			, TextureFilter::Linear
			, TextureFilter::Linear
			, DataMemFormat::UByte)
		, mSize(0)
	{
		mDatas[0] = nullptr;
		mDatas[1] = nullptr;
		mDatas[2] = nullptr;
		mDatas[3] = nullptr;
		mDatas[4] = nullptr;
		mDatas[5] = nullptr;
	}

	TextureCube::TextureCube(std::string name)
		: TextureCube()
	{
		mDatas[0] = nullptr;
		mDatas[1] = nullptr;
		mDatas[2] = nullptr;
		mDatas[3] = nullptr;
		mDatas[4] = nullptr;
		mDatas[5] = nullptr;
		mName.assign(std::move(name));
		mEngineName = mName;
	}

	TextureCube::~TextureCube()
	{
		Graphics::getInstance()->addCommand(new RenderCMD_DeleteTextureCube(this));
	}

	void TextureCube::setImage(const std::array<Image*, 6>& images)
	{
		TINY_ASSERT(images[0]->getWidth() == images[0]->getHeight());

		mSize = images[0]->getWidth();

		mAttachPosition = TextureAttachPosition::ColorComponent;
		mMinFilter = ContextMap::TextureFilterArray[(uint32_t)TextureFilter::Linear];
		mMagFilter = ContextMap::TextureFilterArray[(uint32_t)TextureFilter::Linear];
		mInternalFormat = getTextureInternalFormat(*images[0]);
		mFormat = getTextureFormat(*images[0]);
		mDataMemFormat = ContextMap::DataMemFormatArray[(uint32_t)(images[0]->isHDR() ? DataMemFormat::Float : DataMemFormat::UByte)];

		mWrapS = ContextMap::TextureWrapArray[(uint32_t)TextureWrap::Clamp_To_Edge];
		mWrapT = ContextMap::TextureWrapArray[(uint32_t)TextureWrap::Clamp_To_Edge];
		mWrapR = ContextMap::TextureWrapArray[(uint32_t)TextureWrap::Clamp_To_Edge];

		for (size_t i = 0; i < images.size(); i++)
		{
			auto size = images[i]->getDataSize();
			auto temp = realloc(mDatas[i], size);
			if (temp)
			{
				mDatas[i] = temp;
				memcpy(mDatas[i], images[i]->getData(), size);
			}
		}
	}

	void TextureCube::setConfig(const uint32_t& size
		, const TextureInternalFormat& internalFormat, const TextureFormat& format
		, const DataMemFormat& dataType
		, const TextureFilter& min, const TextureFilter& mag
		, const TextureWrap& wrapS, const TextureWrap& wrapT, const TextureWrap& wrapR)
	{
		mSize = size;

		mInternalFormat = ContextMap::TextureInternalFormatArray[(uint32_t)internalFormat];
		mFormat = ContextMap::TextureFormatArray[(uint32_t)format];
		mDataMemFormat = ContextMap::DataMemFormatArray[(uint32_t)dataType];

		mMinFilter = ContextMap::TextureFilterArray[(uint32_t)min];
		mMagFilter = ContextMap::TextureFilterArray[(uint32_t)mag];

		mWrapS = ContextMap::TextureWrapArray[(uint32_t)wrapS];
		mWrapT = ContextMap::TextureWrapArray[(uint32_t)wrapT];
		mWrapR = ContextMap::TextureWrapArray[(uint32_t)wrapR];
	}

	void TextureCube::generate()
	{
		Graphics::getInstance()->addCommand(new RenderCMD_CreateTextureCube(this));
	}

	void TextureCube::apply(uint32_t id)
	{
		Texture::apply(id);
		for (auto& d : mDatas)
		{
			free(d);
			d = nullptr;
		}
	}

	//--------------------------------------------------------
	//
	//	TextureRender2D
	//
	TINY_OBJECT_CPP(TextureRender2D, Texture);
	TextureRender2D::TextureRender2D()
		: Base(TextureFormat::RGBA
			, TextureFormat::RGBA
			, TextureFilter::Linear
			, TextureFilter::Linear
			, DataMemFormat::UByte)
		, mWidth(0)
		, mHeight(0)
	{

	}

	TextureRender2D::TextureRender2D(std::string name)
		: Base(TextureFormat::RGBA
			, TextureFormat::RGBA
			, TextureFilter::Linear
			, TextureFilter::Linear
			, DataMemFormat::UByte)
		, mWidth(0)
		, mHeight(0)
	{
		mName.assign(name);
		mEngineName = mName;
	}

	TextureRender2D::~TextureRender2D()
	{
		Graphics::getInstance()->addCommand(new RenderCMD_DeleteTextureRender2D(this));
	}

	void TextureRender2D::setConfig(const uint32_t& width, const uint32_t& height, const TextureInternalFormat& internalFormat)
	{
		mWidth = width;
		mHeight = height;
		mInternalFormat = ContextMap::TextureInternalFormatArray[(uint32_t)internalFormat];
	}

	void TextureRender2D::generate()
	{
		Graphics::getInstance()->addCommand(new RenderCMD_CreateTextureRender2D(this));
	}
}

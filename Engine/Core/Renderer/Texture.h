#pragma once
#include "../Head/CppHead.h"
#include "../Head/Context.h"
#include "../Head/ConfigHead.h"

namespace tezcat::Tiny::Core
{
	class Image;
	class TINY_API Texture
	{
	public:
		Texture();
		Texture(const TextureChannel& internalChannel, const TextureWrap& wrap, const TextureFilter& filter);
		virtual ~Texture();

		virtual TextureType getTextureType() const = 0;
		virtual void createTexture(Image* image) = 0;
		virtual void createTexture(const std::string& filePath);

		uint32_t getTextureID() const { return mTextureID; }

		const TexWrapWrapper& getWrap() const { return mWrap; }
		void setWrap(const TextureWrap& val)
		{
			mWrap = ContextMap::TextureWrapArray[(int)val];
		}

		const TexFilterWrapper& getFilter() const { return mFilter; }
		void setFilter(const TextureFilter& val)
		{
			mFilter = ContextMap::TextureFilterArray[(int)val];
		}

		const TexChannelWrapper& getInternalChannel() const { return mInternalChannel; }
		const TexChannelWrapper& getChannel() const { return mChannel; }
		const DataTypeWrapper& getDataType() const { return mDataType; }

	public:
		static TexChannelWrapper getTextureChannels(Image* image);

	protected:
		uint32_t mTextureID;
		TexWrapWrapper mWrap;
		TexFilterWrapper mFilter;
		TexChannelWrapper mInternalChannel;
		TexChannelWrapper mChannel;
		DataTypeWrapper mDataType;
	};

	class TINY_API Texture2D : public Texture
	{
	public:
		TextureType getTextureType() const override { return TextureType::Texture2D; }
	};

	class TINY_API Texture3D : public Texture
	{
	public:
		TextureType getTextureType() const final { return TextureType::Texture3D; }
	};

	class TINY_API TextureCube : public Texture
	{
	public:
		TextureType getTextureType() const final { return TextureType::TextureCube; }

	private:
		void createTexture(Image* image) final {}
	};

	enum class TINY_API TextureBufferType : uint32_t
	{
		Color = 0,
		Depth,
		Stencil,
		DepthAndStencil,
	};

	struct TINY_API TextureBufferInfo
	{
		TextureBufferType bufferType;
		TextureChannel internalChannel;
		TextureChannel channel;
		DataType dataType;
		bool isCache;

		TextureBufferInfo(const TextureBufferType& bufferType, const TextureChannel& internalChannel)
			: TextureBufferInfo(bufferType, internalChannel, internalChannel, DataType::UByte, true)
		{

		}

		TextureBufferInfo(const TextureBufferType& bufferType, const TextureChannel& internalChannel, const TextureChannel& channel, const DataType& dataType, bool isCache = false)
			: bufferType(bufferType)
			, internalChannel(internalChannel)
			, channel(channel)
			, dataType(dataType)
			, isCache(isCache)
		{

		}
	};

	/// <summary>
	/// WriteOnlyBuffer
	/// Used By FrameBuffer
	/// </summary>
	class TINY_API TextureRenderBuffer2D : public Texture2D
	{
	public:
		TextureType getTextureType() const override { return TextureType::TextureRenderBuffer2D; }
		virtual void createTexture(const int& width, const int& high, const TextureChannel& interanlChannel) = 0;

	private:
		void createTexture(Image* image) final {}
	};

	/// <summary>
	/// Used By FrameBuffer
	/// </summary>
	class TINY_API TextureBuffer2D : public TextureRenderBuffer2D
	{
	public:
		TextureType getTextureType() const final { return TextureType::TextureBuffer2D; }
		virtual void createTexture(const int& width, const int& high, const TextureChannel& interanlChannel, const TextureChannel& channel, const DataType& dataType) = 0;
	};


}

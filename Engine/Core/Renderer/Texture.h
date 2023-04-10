#pragma once
#include "../Head/CppHead.h"
#include "../Head/Context.h"
#include "../Head/ConfigHead.h"

namespace tezcat::Tiny::Core
{
	class Image;

	struct TINY_API TextureInfo
	{
		std::string name;
		TextureFilter filter;
		TextureWrap wrap;
		TextureChannel internalChannel;
		TextureChannel channel;
		DataType dataType;
		bool isManagered;

		/// <summary>
		/// Skybox Used
		/// </summary>
		TextureInfo(const std::string& name)
			: TextureInfo(name
				, TextureFilter::Linear
				, TextureWrap::Clamp_To_Edge
				, TextureChannel::None
				, TextureChannel::None
				, DataType::UByte
				, true)
		{

		}

		/// <summary>
		/// Texture2D
		/// </summary>
		TextureInfo(const std::string& name
			, const TextureChannel& internalChannel
			, const TextureWrap& wrap = TextureWrap::Clamp_To_Edge
			, const TextureFilter& filter = TextureFilter::Linear)
			: TextureInfo(name
				, filter
				, wrap
				, internalChannel
				, TextureChannel::None
				, DataType::UByte
				, true)
		{

		}

		TextureInfo(const std::string& name
			, const TextureFilter& filter
			, const TextureWrap& wrap
			, const TextureChannel& internalChannel
			, const TextureChannel& channel
			, const DataType& dataType
			, bool isManagered = true)
			: name(name)
			, filter(filter)
			, wrap(wrap)
			, internalChannel(internalChannel)
			, channel(channel)
			, dataType(dataType)
			, isManagered(isManagered)
		{

		}
	};

	enum class TINY_API TextureBufferType : uint32_t
	{
		ColorComponent = 0,
		DepthComponent,
		StencilCompoent,
		DepthStencilComponent,
	};

	struct TINY_API TextureBufferInfo : TextureInfo
	{
		TextureBufferType bufferType;
		bool isCache;

		/// <summary>
		/// RenderBuffer Used
		/// </summary>
		TextureBufferInfo(const std::string& name
			, const TextureBufferType& bufferType
			, const TextureChannel& internalChannel)
			: TextureBufferInfo(name
				, bufferType
				, TextureFilter::Linear
				, TextureWrap::Clamp_To_Edge
				, internalChannel
				, internalChannel
				, DataType::UByte
				, true
				, false)
		{

		}

		/// <summary>
		/// ColorBuffer
		/// </summary>
		TextureBufferInfo(const std::string& name
			, const TextureBufferType& bufferType
			, const TextureChannel& internalChannel
			, const TextureChannel& channel
			, const DataType& dataType
			, bool isManagered)
			: TextureBufferInfo(name
				, bufferType
				, TextureFilter::Linear
				, TextureWrap::Clamp_To_Edge
				, internalChannel
				, channel
				, dataType
				, false
				, isManagered)

		{

		}

		TextureBufferInfo(const std::string& name
			, const TextureBufferType& bufferType
			, const TextureFilter& filter
			, const TextureWrap& warp
			, const TextureChannel& internalChannel
			, const TextureChannel& channel
			, const DataType& dataType
			, bool isCache
			, bool isManagered)
			: TextureInfo(name, filter, warp, internalChannel, channel, dataType, isManagered)
			, bufferType(bufferType)
			, isCache(isCache)

		{

		}
	};

	class TINY_API Texture
	{
	public:
		Texture();
		/// <summary>
		/// 默认情况下
		/// Texture都是由生成他的东西负责生命周期
		/// </summary>
		Texture(const TextureChannel& internalChannel
			, const TextureWrap& wrap
			, const TextureFilter& filter
			, const bool& isManagered = false);
		virtual ~Texture();

		virtual TextureType getTextureType() const = 0;
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

		const uint32_t getUID() const { return mUID; }

		bool isManagered() { return mIsManagered; }
		void setManagered(bool value) { mIsManagered = value; }

	public:
		static TexChannelWrapper getTextureChannels(const Image& image);

	protected:
		uint32_t mUID;
		uint32_t mTextureID;
		TexWrapWrapper mWrap;
		TexFilterWrapper mFilter;
		TexChannelWrapper mInternalChannel;
		TexChannelWrapper mChannel;
		DataTypeWrapper mDataType;
		bool mIsManagered;

	private:
		static uint32_t sUIDGiver;
		static std::deque<uint32_t> sFreeUIDs;
		static uint32_t giveUID();
	};

	class TINY_API Texture2D : public Texture
	{
	public:
		Texture2D();
		~Texture2D() = default;
		TextureType getTextureType() const override { return TextureType::Texture2D; }
		virtual void create(const Image& img, const TextureInfo& info) = 0;
		void setSize(const int& width, const int& height)
		{
			mWidth = width;
			mHeight = height;
		}

		uint32_t getWidth() { return mWidth; }
		uint32_t getHeight() { return mHeight; }

	protected:
		uint32_t mWidth;
		uint32_t mHeight;
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
		virtual void create(const std::array<Image, 6>& images, const TextureInfo& info) = 0;
	};

	/// <summary>
	/// WriteOnlyBuffer
	/// Used By FrameBuffer
	/// </summary>
	class TINY_API TextureRenderBuffer2D : public Texture2D
	{
	public:
		TextureType getTextureType() const override { return TextureType::TextureRenderBuffer2D; }
		virtual void create(const int& width
							, const int& high
							, const TextureChannel& interanlChannel) = 0;

		const TextureBufferType& getBufferType() const { return mBufferType; }
		void setBufferType(const TextureBufferType& val) { mBufferType = val; }

	private:
		void create(const Image& img, const TextureInfo& info) final;

	protected:
		TextureBufferType mBufferType;

	};

	/// <summary>
	/// Used By FrameBuffer
	/// </summary>
	class TINY_API TextureBuffer2D : public TextureRenderBuffer2D
	{
	public:
		TextureType getTextureType() const final { return TextureType::TextureBuffer2D; }
		virtual void create(const int& width
							, const int& high
							, const TextureChannel& internalChannel
							, const TextureChannel& channel
							, const DataType& dataType) = 0;

		virtual void create(const int& width, const int& high, const TextureBufferInfo& info) = 0;
	};


}

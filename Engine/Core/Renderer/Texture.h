#pragma once
#include "../Head/CppHead.h"
#include "../Head/RenderConfig.h"
#include "../Head/ConfigHead.h"
#include "../Base/TinyObject.h"

namespace tezcat::Tiny
{
	class Image;

	struct TINY_API TextureInfo
	{
		std::string name;
		TextureType type;
		TextureAttachPosition attachPosition;
		TextureFilter filter;
		TextureWrap wrap;
		TextureChannel internalChannel;
		TextureChannel channel;
		DataType dataType;

		/*
		* @author HCL
		* @info 2023|5|17
		* @brief name填空字符串表示不将此tex保存在manager中
		*/
		TextureInfo(const std::string& name
				  , const TextureType& type
				  , const TextureAttachPosition& attachPosition				  , const TextureFilter& filter				  , const TextureWrap& wrap				  , const TextureChannel& internalChannel				  , const TextureChannel& channel				  , const DataType& dataType)
			: name(name)
			, type(type)
			, attachPosition(attachPosition)			, filter(filter)			, wrap(wrap)			, internalChannel(internalChannel)			, channel(channel)			, dataType(dataType)
		{

		}
		/*
		* @author HCL
		* @info 2023|5|17
		* @brief name填空字符串表示不将此tex保存在manager中
		* @brief 创建Skybox
		*/
		TextureInfo(const std::string& name)
			: TextureInfo(name
						, TextureType::TextureCube
						, TextureAttachPosition::ColorComponent
						, TextureFilter::Linear
						, TextureWrap::Repeat
						, TextureChannel::None
						, TextureChannel::None
						, DataType::UByte)
		{

		}

		/*
		* @author HCL
		* @info 2023|5|17
		* @brief name填空字符串表示不将此tex保存在manager中
		*/
		TextureInfo(const std::string& name
				  , const TextureChannel& internalChannel
				  , const TextureWrap& wrap = TextureWrap::Repeat
				  , const TextureFilter& filter = TextureFilter::Linear)
			: TextureInfo(name
						, TextureType::Texture2D
						, TextureAttachPosition::ColorComponent
						, filter
						, wrap
						, internalChannel
						, internalChannel
						, DataType::UByte)
		{

		}

		/*
		* @author HCL
		* @info 2023|5|17
		* @brief name填空字符串表示不将此tex保存在manager中
		* @brief 创建RenderBuffer
		*/
		TextureInfo(const std::string& name
				  , const TextureType& type
				  , const TextureAttachPosition& attachPosition
				  , const TextureChannel& internalChannel)
			: TextureInfo(name
						, type
						, attachPosition
						, TextureFilter::Linear
						, TextureWrap::Repeat
						, internalChannel
						, internalChannel
						, DataType::UByte)
		{

		}

		/*
		* @author HCL
		* @info 2023|5|17
		* @brief name填空字符串表示不将此tex保存在manager中
		* @brief 创建ColorBuffer
		*/
		TextureInfo(const std::string& name
				  , const TextureAttachPosition& attachPosition
				  , const TextureChannel& internalChannel
				  , const TextureChannel& channel
				  , const DataType& dataType)
			: TextureInfo(name
						, TextureType::Texture2D
						, attachPosition
						, TextureFilter::Linear
						, TextureWrap::Repeat
						, internalChannel
						, channel
						, dataType)
		{

		}

		/*
		* @author HCL
		* @info 2023|5|17
		* @brief name填空字符串表示不将此tex保存在manager中
		*/
		TextureInfo(const std::string& name
				  , const TextureType type
				  , const TextureAttachPosition& attachPosition
				  , const TextureChannel& internalChannel
				  , const TextureChannel& channel
				  , const DataType& dataType)
			: TextureInfo(name
						, TextureType::Texture2D
						, attachPosition
						, TextureFilter::Linear
						, TextureWrap::Repeat
						, internalChannel
						, channel
						, dataType)
		{

		}
	};

	class TINY_API Texture : public TinyObject
	{
		TINY_RTTI_H(Texture);
	public:
		Texture();

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

		const TextureAttachPosition& getAttachPosition() const { return mAttachPosition; }
		void setAttachPosition(const TextureAttachPosition& val) { mAttachPosition = val; }

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
		TextureAttachPosition mAttachPosition;
		bool mIsManagered;

	private:
		static uint32_t sUIDGiver;
		static std::deque<uint32_t> sFreeUIDs;
		static uint32_t giveUID();
	};

	class TINY_API Texture2D : public Texture
	{
		TINY_RTTI_H(Texture2D);
	public:
		Texture2D();
		virtual ~Texture2D() = default;

		TextureType getTextureType() const override { return TextureType::Texture2D; }
		virtual void create(const Image& img, const TextureInfo& info) {};
		virtual void create(const int& width, const int& height
						   , const TextureChannel& internalChannel
						   , const TextureChannel& channel
						   , const DataType& dataType) {};
		virtual void create(const int& width, const int& height, const TextureInfo& info) {};

		void setSize(const int& width, const int& height)
		{
			mWidth = width;
			mHeight = height;
		}

		uint32_t getWidth() const { return mWidth; }
		uint32_t getHeight() const { return mHeight; }

	protected:
		uint32_t mWidth;
		uint32_t mHeight;
	};

	class TINY_API Texture3D : public Texture
	{
		TINY_RTTI_H(Texture3D);
	public:
		virtual ~Texture3D() = default;

		TextureType getTextureType() const final { return TextureType::Texture3D; }
	};

	class TINY_API TextureCube : public Texture
	{
		TINY_RTTI_H(TextureCube);
	public:
		virtual ~TextureCube() = default;

		TextureType getTextureType() const final { return TextureType::TextureCube; }
		virtual void create(const std::array<Image, 6>& images, const TextureInfo& info) = 0;
		virtual void create(const int& width, const int& hegiht, const TextureInfo& info) = 0;

	};

	/// <summary>
	/// WriteOnlyBuffer
	/// Used By FrameBuffer
	/// </summary>
	class TINY_API TextureRender2D : public Texture2D
	{
		TINY_RTTI_H(TextureRender2D);
	public:
		virtual ~TextureRender2D() = default;

		TextureType getTextureType() const override { return TextureType::TextureRender2D; }
		virtual void create(const int& width, const int& height, const TextureChannel& interanlChannel) = 0;


	private:
		void create(const Image& img, const TextureInfo& info) final {};
	};
}

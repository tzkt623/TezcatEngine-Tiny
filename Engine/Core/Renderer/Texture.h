#pragma once
#include "../Head/TinyCpp.h"
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
		TextureFilter minFilter;
		TextureFilter magFilter;
		TextureWrap wrapS;
		TextureWrap wrapT;
		TextureWrap wrapR;
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
				  , const TextureAttachPosition& attachPosition				  , const TextureFilter& minFilter
				  , const TextureFilter& magFilter				  , const TextureWrap& wrapS
				  , const TextureWrap& wrapT
				  , const TextureWrap& wrapR				  , const TextureChannel& internalChannel				  , const TextureChannel& channel				  , const DataType& dataType)
			: name(name)
			, type(type)
			, attachPosition(attachPosition)			, minFilter(minFilter)
			, magFilter(magFilter)			, wrapS(wrapS)
			, wrapT(wrapT)
			, wrapR(wrapR)			, internalChannel(internalChannel)			, channel(channel)			, dataType(dataType)
		{
		}

		TextureInfo(const TextureType& type
				  , const TextureAttachPosition& attachPosition				  , const TextureFilter& minFilter
				  , const TextureFilter& magFilter				  , const TextureWrap& wrapS
				  , const TextureWrap& wrapT
				  , const TextureWrap& wrapR				  , const TextureChannel& internalChannel				  , const TextureChannel& channel				  , const DataType& dataType)
			: name()
			, type(type)
			, attachPosition(attachPosition)			, minFilter(minFilter)
			, magFilter(magFilter)			, wrapS(wrapS)
			, wrapT(wrapT)
			, wrapR(wrapR)			, internalChannel(internalChannel)			, channel(channel)			, dataType(dataType)
		{
		}

		/*
		* @author HCL
		* @info 2023|5|18
		* @brief 创建2D专用
		*/
		TextureInfo(const std::string& name
				  , const TextureType& type
				  , const TextureAttachPosition& attachPosition				  , const TextureFilter& minFilter
				  , const TextureFilter& magFilter				  , const TextureWrap& wrapS
				  , const TextureWrap& wrapT				  , const TextureChannel& internalChannel				  , const TextureChannel& channel				  , const DataType& dataType)
			: name(name)
			, type(type)
			, attachPosition(attachPosition)			, minFilter(minFilter)
			, magFilter(magFilter)			, wrapS(wrapS)
			, wrapT(wrapT)
			, wrapR(TextureWrap::Repeat)			, internalChannel(internalChannel)			, channel(channel)			, dataType(dataType)
		{

		}

		TextureInfo(const TextureType& type
				  , const TextureAttachPosition& attachPosition				  , const TextureFilter& minFilter
				  , const TextureFilter& magFilter				  , const TextureWrap& wrapS
				  , const TextureWrap& wrapT				  , const TextureChannel& internalChannel				  , const TextureChannel& channel				  , const DataType& dataType)
			: name()
			, type(type)
			, attachPosition(attachPosition)			, minFilter(minFilter)
			, magFilter(magFilter)			, wrapS(wrapS)
			, wrapT(wrapT)
			, wrapR(TextureWrap::Repeat)			, internalChannel(internalChannel)			, channel(channel)			, dataType(dataType)
		{

		}

		/*
		* @author HCL
		* @info 2023|5|18
		* @brief image创建2D专用
		*/
		TextureInfo(const std::string& name				  , const TextureFilter& minFilter
				  , const TextureFilter& magFilter				  , const TextureWrap& wrapS
				  , const TextureWrap& wrapT)
			: name(name)
			, type(TextureType::Texture2D)
			, attachPosition(TextureAttachPosition::ColorComponent)			, minFilter(minFilter)
			, magFilter(magFilter)			, wrapS(wrapS)
			, wrapT(wrapT)
			, wrapR(TextureWrap::Repeat)			, internalChannel(TextureChannel::RGB)			, channel(TextureChannel::RGB)			, dataType(DataType::UByte)
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
						, TextureFilter::Linear
						, TextureWrap::Repeat
						, TextureWrap::Repeat
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
						, filter
						, wrap
						, wrap
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
						, TextureFilter::Linear
						, TextureWrap::Repeat
						, TextureWrap::Repeat
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
						, TextureFilter::Linear
						, TextureWrap::Repeat
						, TextureWrap::Repeat
						, TextureWrap::Repeat
						, internalChannel
						, channel
						, dataType)
		{

		}

		TextureInfo(const TextureAttachPosition& attachPosition
		  , const TextureChannel& internalChannel
		  , const TextureChannel& channel
		  , const DataType& dataType)
			: TextureInfo(""
						, TextureType::Texture2D
						, attachPosition
						, TextureFilter::Linear
						, TextureFilter::Linear
						, TextureWrap::Repeat
						, TextureWrap::Repeat
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
						, TextureFilter::Linear
						, TextureWrap::Repeat
						, TextureWrap::Repeat
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
		Texture(const TextureAttachPosition& attachPosition
			  , const TextureChannel& internalChannel
			  , const TextureChannel& channel
			  , const TextureFilter& minFilter
			  , const TextureFilter& magFilter
			  , const DataType& dataType);
		virtual ~Texture();

		virtual void generate() = 0;
		virtual void update() {};
		virtual TextureType getTextureType() const = 0;

		const uint32_t& getTextureID() const { return mTextureID; }
		virtual void apply(uint32_t id) { mTextureID = id; }

		const TexChannelWrapper& getInternalChannel() const { return mInternalChannel; }
		const TexChannelWrapper& getChannel() const { return mChannel; }
		const DataTypeWrapper& getDataType() const { return mDataType; }

		const uint32_t& getUID() const { return mUID; }
		const std::string& getName() const { return mName; }
		void setName(const std::string& name) { mName = name; }

		const TextureAttachPosition& getAttachPosition() const { return mAttachPosition; }
		void setAttachPosition(const TextureAttachPosition& val) { mAttachPosition = val; }

		const TexFilterWrapper& getMinFilter() const { return mMinFilter; }
		const TexFilterWrapper& getMagFilter() const { return mMagFilter; }

		void setMinFilter(TextureFilter filter)
		{
			mMinFilter = ContextMap::TextureFilterArray[(uint32_t)filter];
		}

		void setMagFilter(TextureFilter filter)
		{
			mMagFilter = ContextMap::TextureFilterArray[(uint32_t)filter];
		}

	public:
		static TexChannelWrapper getTextureChannels(const Image& image);

	protected:
		std::string mName;
		uint32_t mUID;
		uint32_t mTextureID;
		TextureAttachPosition mAttachPosition;
		TexFilterWrapper mMinFilter;
		TexFilterWrapper mMagFilter;
		TexChannelWrapper mInternalChannel;
		TexChannelWrapper mChannel;
		DataTypeWrapper mDataType;

	private:
		static uint32_t sUIDGiver;
		static std::deque<uint32_t> sFreeUIDs;
		static uint32_t giveUID();
	};

	class TINY_API Texture2D : public Texture
	{
		Texture2D();
		Texture2D(const std::string& name);
		TINY_RTTI_H(Texture2D);
		TINY_Factory(Texture2D);
	public:
		virtual ~Texture2D();

		TextureType getTextureType() const override { return TextureType::Texture2D; }
		virtual void setData(const int& width, const int& height, const TextureInfo& info);
		virtual void setData(const Image* image, const TextureInfo& info);
		virtual void setData(const Image* image);
		virtual void updateData(const Image* image);

		void generate() override;
		void update() override;

		virtual void setSize(const int& width, const int& height)
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
		TINY_RTTI_H(Texture3D);
		TINY_Factory(Texture3D);

	public:
		virtual ~Texture3D();

		TextureType getTextureType() const final { return TextureType::Texture3D; }

	protected:
		TexWrapWrapper mWrapS;
		TexWrapWrapper mWrapT;
		TexWrapWrapper mWrapR;
	};

	class TINY_API TextureCube : public Texture
	{
		TextureCube();
		TextureCube(const std::string& name);
		TINY_RTTI_H(TextureCube);
		TINY_Factory(TextureCube);

	public:
		virtual ~TextureCube();

		TextureType getTextureType() const final { return TextureType::TextureCube; }

		virtual void setData(const std::array<Image*, 6>& images, const TextureInfo& info);
		virtual void setData(const std::array<Image*, 6>& images);
		virtual void setData(const int& width, const int& height, const TextureInfo& info);
		virtual void generate();

		const TexWrapWrapper& getWrapS() const { return mWrapS; }
		const TexWrapWrapper& getWrapT() const { return mWrapT; }
		const TexWrapWrapper& getWrapR() const { return mWrapR; }
		uint32_t getWidth() const { return mWidth; }
		uint32_t getHeight() const { return mHeight; }

		void setSize(const int& width, const int& height)
		{
			mWidth = width;
			mHeight = height;
		}

		void* getData(uint32_t index) const { return mDatas[index]; }

		void apply(uint32_t id);

	protected:
		uint32_t mWidth;
		uint32_t mHeight;
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
		TextureRender2D(const std::string& name);
		TINY_RTTI_H(TextureRender2D);
		TINY_Factory(TextureRender2D);
	public:
		virtual ~TextureRender2D();

		TextureType getTextureType() const override { return TextureType::TextureRender2D; }
		virtual void setData(const int& width, const int& height, const TextureChannel& interanlChannel) {}
		virtual void setData(const int& width, const int& height, const TextureInfo& info);
		void generate() override;

		uint32_t getWidth() const { return mWidth; }
		uint32_t getHeight() const { return mHeight; }

		const TexWrapWrapper& getWrapS() const { return mWrapS; }
		const TexWrapWrapper& getWrapT() const { return mWrapT; }

	protected:
		uint32_t mWidth;
		uint32_t mHeight;
		TexWrapWrapper mWrapS;
		TexWrapWrapper mWrapT;
	};
}

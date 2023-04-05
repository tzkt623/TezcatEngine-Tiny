#pragma once

#include "CppHead.h"
#include "ConfigHead.h"

namespace tezcat::Tiny::Core
{
	enum class TINY_API DataType : uint32_t
	{
		Byte = 0,
		UByte,
		Short,
		UShort,
		Int32,
		UInt32,
		Int64,
		UInt64,
		Float32,
		Float64,
		UInt_24_8,

		Count
	};

	enum class TINY_API TextureType : uint32_t
	{
		Texture1D = 0,
		Texture2D,
		Texture3D,
		TextureCube,
		Texture1DA,
		Texture2DA,
		TextureBuffer2D,
		TextureRenderBuffer2D,

		Count
	};

	enum class TINY_API TextureWrap : uint32_t
	{
		Repeat = 0,
		Mirrored_Repeat,
		Clamp_To_Edge,
		Clamp_To_Border,

		Count
	};

	enum class TINY_API TextureFilter : uint32_t
	{
		Nearest = 0,
		Linear,

		Count
	};

	enum class TINY_API TextureChannel : uint32_t
	{
		None = 0,
		R,
		RG,
		RGB,
		RGBA,
		//internal only
		Depth16,
		//internal only
		Depth24,
		//internal only
		Depth32,
		//internal only
		Depth32f,
		//not internal only
		Depth_Stencil,
		//internal only
		Depth24_Stencil8,
		//internal only
		Depth32f_Stencil8,
		//internal only
		Stencil8,

		Count
	};

	enum class TINY_API DrawMode : uint32_t
	{
		Points = 0,
		Lines,
		Lines_Loop,
		Lines_Strip,
		Triangles,
		Triangles_Strip,
		Triangles_Fan,

		Count
	};

	enum class TINY_API CullFace : uint32_t
	{
		Off = 0,
		Back,
		Front,
		All,

		Count
	};

	enum class TINY_API DepthTest : uint32_t
	{
		Off = 0,
		Always,
		Never,
		Less,
		LessEqual,
		Greater,
		GreaterEqual,
		Equal,
		NotEqual,

		Count
	};

	enum class TINY_API Blend : uint32_t
	{
		Zero = 0,
		One,
		Source,
		One_Minus_Source,
		Target,
		One_Minus_Target,
		SourceAlpha,
		One_Minus_SourceAlpha,
		TargetAlpha,
		One_Minus_TargetAlpha,
		Const,
		One_Minus_Const,
		ConstAlpha,
		One_Minus_ConstAlpha,

		Count
	};

	enum class TINY_API LightMode : uint32_t
	{
		Unlit,
		Main,
		Addition
	};

	enum TINY_API VertexMask : uint32_t
	{
		Position = 0,
		Normal,
		Color,
		UV
	};

	template<typename TinyType, typename PlatformType>
	struct TINY_API TinyValueConventor
	{
		TinyType tiny;
		PlatformType platform;

		TinyValueConventor()
		{

		}

		TinyValueConventor(const TinyValueConventor& other)
			: tiny(other.tiny)
			, platform(other.platform)
		{

		}

		TinyValueConventor(const TinyType& tiny, const PlatformType& platform)
			: tiny(tiny)
			, platform(platform)
		{

		}

		TinyValueConventor& operator=(const TinyValueConventor& other)
		{
			tiny = other.tiny;
			platform = other.platform;
			return *this;
		}
	};

	using DataTypeWrapper = TinyValueConventor<DataType, uint32_t>;
	using TexTypeWrapper = TinyValueConventor<TextureType, uint32_t>;
	using TexWrapWrapper = TinyValueConventor<TextureWrap, uint32_t>;
	using TexFilterWrapper = TinyValueConventor<TextureFilter, uint32_t>;
	using TexChannelWrapper = TinyValueConventor<TextureChannel, uint32_t>;
	using DrawModeWrapper = TinyValueConventor<DrawMode, uint32_t>;
	using CullFaceWrapper = TinyValueConventor<CullFace, uint32_t>;
	using BlendWrapper = TinyValueConventor<Blend, uint32_t>;
	using DepthTestWrapper = TinyValueConventor<DepthTest, uint32_t>;

	struct TINY_API ContextMap
	{
		static std::array<DataTypeWrapper, (std::size_t)DataType::Count> DataTypeArray;

		/// <summary>
		///	Texture1D
		/// Texture2D
		///	Texture3D
		///	TextureCube
		///	Texture1DA
		///	Texture2DA
		/// </summary>
		static std::array<TexTypeWrapper, (std::size_t)TextureType::Count> TextureTypeArray;

		/// <summary>
		/// Tex_REPEAT
		/// Tex_MIRRORED_REPEAT
		///	Tex_CLAMP_TO_EDGE
		///	Tex_CLAMP_TO_BORDER
		/// </summary>
		static std::array<TexWrapWrapper, (std::size_t)TextureWrap::Count> TextureWrapArray;

		/// <summary>
		/// Tex_NEAREST
		/// Tex_LINEAR
		/// </summary>
		static std::array<TexFilterWrapper, (std::size_t)TextureFilter::Count> TextureFilterArray;

		/// <summary>
		/// Tex_RGB
		/// Tex_RGBA
		/// </summary>
		static std::array<TexChannelWrapper, (std::size_t)TextureChannel::Count> TextureChannelArray;

		/// <summary>
		/// Points
		/// Lines
		///	Lines_Loop
		///	Lines_Strip
		///	Triangles
		///	Triangles_Strip
		///	Triangles_Fan
		/// </summary>
		static std::array<DrawModeWrapper, (std::size_t)DrawMode::Count> DrawModeArray;

		static std::unordered_map<std::string, CullFaceWrapper> CullFaceMap;
		static std::array<CullFaceWrapper, (std::size_t)CullFace::Count> CullFaceArray;

		static std::unordered_map<std::string, BlendWrapper> BlendMap;
		static std::array<BlendWrapper, (std::size_t)Blend::Count> BlendArray;

		static std::unordered_map<std::string, DepthTestWrapper> DepthTestMap;
		static std::array<DepthTestWrapper, (std::size_t)DepthTest::Count> DepthTestArray;

		static std::unordered_map<std::string, LightMode> LightModeMap;
	};

	enum class TINY_API PrimitiveDrawType
	{
		Vertex,
		Index,
	};

	enum class TINY_API PrimitiveDataMode
	{
		USHORT,
		SHORT,
		UINT,
		INT
	};

	struct TINY_API PrimitiveConfig
	{
		PrimitiveDrawType drawType;
		PrimitiveDataMode dataMode;
	};
}

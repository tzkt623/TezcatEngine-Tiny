#pragma once

#include "CppHead.h"
#include "ConfigHead.h"

namespace tezcat::Tiny::Core
{
	enum class TINY_API TextureType : uint32_t
	{
		Texture1D = 0,
		Texture2D,
		Texture3D,
		TextureCube,
		Texture1DA,
		Texture2DA,

		Count
	};

	enum class TINY_API TextureWrap : uint32_t
	{
		Tex_REPEAT = 0,
		Tex_MIRRORED_REPEAT,
		Tex_CLAMP_TO_EDGE,
		Tex_CLAMP_TO_BORDER,

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

	enum class TINY_API TextureFilter : uint32_t
	{
		Tex_NEAREST = 0,
		Tex_LINEAR,

		Count
	};

	enum class TINY_API TextureChannel : uint32_t
	{
		Tex_RGB = 0,
		Tex_RGBA,

		Count
	};

	enum class TINY_API CullFace : uint8_t
	{
		Off = 0,
		Back,
		Front,
		All,

		Count
	};

	enum class TINY_API Blend
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

	using TexTypeWrapper = TinyValueConventor<TextureType, uint32_t>;
	using TexWrapWrapper = TinyValueConventor<TextureWrap, uint32_t>;
	using TexFilterWrapper = TinyValueConventor<TextureFilter, uint32_t>;
	using TexChannelWrapper = TinyValueConventor<TextureChannel, uint32_t>;
	using DrawModeWrapper = TinyValueConventor<DrawMode, uint32_t>;
	using CullFaceWrapper = TinyValueConventor<CullFace, uint32_t>;
	using BlendWrapper = TinyValueConventor<Blend, uint32_t>;

	struct TINY_API ContextMap
	{
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
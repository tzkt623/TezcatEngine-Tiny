#pragma once

#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"
#include "../Head/GLMHead.h"

#include "UniformBufferConfig.h"

namespace tezcat::Tiny
{
	enum class TINY_API UniformType : int8_t
	{
		Error = -1,
		Bool,
		Int, Int2, Int3, Int4,
		UInt, UInt2, UInt3, UInt4,
		Float, Float2, Float3, Float4,
		Double, Double2, Double3, Double4,
		Mat3, Mat4,
		Tex2D, Tex3D, TexCube,
		Struct,
		Count
	};

	enum class TINY_API DataMemFormat : uint8_t
	{
		Byte = 0,
		UByte,
		Short,
		UShort,
		Int32,
		UInt32,
		Int64,
		UInt64,
		Float,
		Float64,
		UInt_24_8,

		Count
	};

	enum class TINY_API TextureType : uint8_t
	{
		Texture1D = 0,
		Texture2D,
		Texture3D,
		TextureCube,
		Texture1DA,
		Texture2DA,
		TextureRender2D,

		Count
	};

	enum class TINY_API TextureWrap : uint8_t
	{
		Repeat = 0,
		Mirrored_Repeat,
		Clamp_To_Edge,
		Clamp_To_Border,

		Count
	};

	enum class TINY_API TextureFilter : uint8_t
	{
		Nearest = 0,
		Linear,
		Linear_Mipmap_Linear,
		Count
	};

	enum class TINY_API TextureInternalFormat : uint8_t
	{
		None = 0,
		//Base
		Depth,
		Depth_Stencil,
		R,
		RG,
		RGB,
		RGBA,
		//8I
		R8,
		RG8,
		RGB8,
		RGBA8,
		R16,
		RG16,
		RGB16,
		RGBA16,
		//16F
		R16F,
		RG16F,
		RGB16F,
		RGBA16F,
		R32F,
		RG32F,
		RGB32F,
		RGBA32F,

		R32UI,

		Count
	};

	enum class TINY_API TextureFormat : uint8_t
	{
		None = 0,
		Depth,
		Stencil,
		Depth_Stencil,
		R,
		RG,
		RGB,
		RGBA,
		BGR,
		BGRA,

		RI,
		Count
	};

	enum class TINY_API TextureAttachPosition : uint8_t
	{
		None = 0,
		ColorComponent,
		DepthComponent,
		StencilCompoent,
		DepthStencilComponent,
	};

	//颜色缓冲区
	enum class TINY_API ColorBuffer : uint8_t
	{
		None = 0,
		A0,
		A1,
		B0,
		B1,
		AX,
		BX,
		X0,
		X1,
		All,

		Count
	};

	enum class TINY_API DrawMode : uint8_t
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

	enum class TINY_API CullFace : uint8_t
	{
		Off = 0,
		Back,
		Front,
		All,

		Count
	};

	enum class TINY_API DepthTest : uint8_t
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

	enum class TINY_API Blend : uint8_t
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

	enum class TINY_API LightMode : uint8_t
	{
		Unlit,
		Forward,
		ForwardAdd,
		Deferred
	};

	enum class TINY_API Queue : uint8_t
	{
		None = 0,
		Prepare,
		Background,
		Opaque,
		AlphaTest,
		OpaqueLast,
		Transparent,
		Overlay,
		Post
	};

	using ClearOptionID = uint32;
	enum TINY_API ClearOption : ClearOptionID
	{
		CO_None = 0,
		CO_Color = 1 << 0,
		CO_Depth = 1 << 1,
		CO_Stencil = 1 << 2,
		CO_Skybox = 1 << 3
	};


	enum class TINY_API RenderPhase : int32_t
	{
		Error = -1,
		Shadow,
		Forward,
		ForwardAdd,
		Deferred,
		ForwardPlus,
	};

	struct TINY_API ViewportInfo
	{
		int OX;
		int OY;
		int Width;
		int Height;

		ViewportInfo()
			: OX(0)
			, OY(0)
			, Width(0)
			, Height(0)
		{

		}

		ViewportInfo(const int& ox, const int& oy, const int& width, const int& height)
			: OX(ox)
			, OY(oy)
			, Width(width)
			, Height(height)
		{

		}
	};

	/*
	* TinyValueConventor
	*
	* @brief 值转换器
	*/
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

	using DataMemFormatWrapper = TinyValueConventor<DataMemFormat, uint32>;
	using TexTypeWrapper = TinyValueConventor<TextureType, uint32>;
	using TexWrapWrapper = TinyValueConventor<TextureWrap, uint32>;
	using TexFilterWrapper = TinyValueConventor<TextureFilter, uint32>;
	using TexInternalFormatWrapper = TinyValueConventor<TextureInternalFormat, uint32>;
	using TexFormatWrapper = TinyValueConventor<TextureFormat, uint32>;
	using ColorBufferWrapper = TinyValueConventor<ColorBuffer, uint32>;
	using DrawModeWrapper = TinyValueConventor<DrawMode, uint32>;
	using CullFaceWrapper = TinyValueConventor<CullFace, uint32>;
	using BlendWrapper = TinyValueConventor<Blend, uint32>;
	using DepthTestWrapper = TinyValueConventor<DepthTest, uint32>;

	struct TINY_API ContextMap
	{
		static std::array<DataMemFormatWrapper, (uint32)DataMemFormat::Count> DataMemFormatArray;

		/// <summary>
		///	Texture1D
		/// Texture2D
		///	Texture3D
		///	TextureCube
		///	Texture1DA
		///	Texture2DA
		/// </summary>
		static std::array<TexTypeWrapper, (uint32)TextureType::Count> TextureTypeArray;

		/// <summary>
		/// Tex_REPEAT
		/// Tex_MIRRORED_REPEAT
		///	Tex_CLAMP_TO_EDGE
		///	Tex_CLAMP_TO_BORDER
		/// </summary>
		static std::array<TexWrapWrapper, (uint32)TextureWrap::Count> TextureWrapArray;

		/// <summary>
		/// Tex_NEAREST
		/// Tex_LINEAR
		/// </summary>
		static std::array<TexFilterWrapper, (uint32)TextureFilter::Count> TextureFilterArray;

		/// <summary>
		/// Tex_RGB
		/// Tex_RGBA
		/// </summary>
		static std::array<TexInternalFormatWrapper, (uint32)TextureInternalFormat::Count> TextureInternalFormatArray;

		/// <summary>
		/// 
		/// </summary>
		static std::array<TexFormatWrapper, (uint32)TextureFormat::Count> TextureFormatArray;

		/// <summary>
		/// 
		/// </summary>
		static std::array<ColorBufferWrapper, (uint32)ColorBuffer::Count> ColorBufferArray;

		/// <summary>
		/// Points
		/// Lines
		///	Lines_Loop
		///	Lines_Strip
		///	Triangles
		///	Triangles_Strip
		///	Triangles_Fan
		/// </summary>
		static std::array<DrawModeWrapper, (uint32)DrawMode::Count> DrawModeArray;

		static std::unordered_map<std::string, CullFaceWrapper> CullFaceMap;
		static std::array<CullFaceWrapper, (uint32)CullFace::Count> CullFaceArray;

		static std::unordered_map<std::string, BlendWrapper> BlendMap;
		static std::array<BlendWrapper, (uint32)Blend::Count> BlendArray;

		static std::unordered_map<std::string, DepthTestWrapper> DepthTestMap;
		static std::array<DepthTestWrapper, (uint32)DepthTest::Count> DepthTestArray;

		static std::unordered_map<std::string, LightMode> LightModeMap;

		static std::unordered_map<std::string, Queue> QueueMap;

		static std::unordered_map<std::string, UniformType> UniformTypeUMap;
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


	enum class MaterialTextureSlot
	{
		AMBIENT = 0,
		TEX_AMBIENT,
		DIFFUSE,
		TEX_DIFFUSE,
		SPECULA,
		TEX_SPECULA,
		NORMALS,
		TEX_NORMALS,
		EMISSIVE,
		TEX_EMISSIVE,
		SHININESS,
		TEX_SHININESS,
		REFLECTION,
		TEX_REFLECTION,
		TRANSPARENT,
		TEX_TRANSPARENT,


		//PBR
		PBR_BASE_COLOR,
		PBR_TEX_BASE_COLOR,

		PBR_NORMAL,
		PBR_TEX_NORMAL,

		PBR_ROUGHNESS,
		PBR_TEX_ROUGHNESS,

		PBR_EMISSION,
		PBR_TEX_EMISSION,

		PBR_AMBIENT_OCCLUSION,
		PBR_TEX_AMBIENT_OCCLUSION,

		PBR_METALNESS,
		PBR_TEX_METALNESS,

		PBR_GLOSSINESS,
		PBR_TEX_GLOSSINESS,
	};

}

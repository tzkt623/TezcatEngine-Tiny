#pragma once

#include "CppHead.h"

namespace tezcat::Tiny::Core
{
	enum class TextureType : uint32_t
	{
		Texture1D = 0,
		Texture2D,
		Texture3D,
		TextureCube,
		Texture1DA,
		Texture2DA,

		Count
	};

	enum class TextureWrap : uint32_t
	{
		Tex_REPEAT = 0,
		Tex_MIRRORED_REPEAT,
		Tex_CLAMP_TO_EDGE,
		Tex_CLAMP_TO_BORDER,

		Count
	};

	enum class DrawMode : uint32_t
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

	enum class TextureFilter : uint32_t
	{
		Tex_NEAREST = 0,
		Tex_LINEAR,

		Count
	};

	enum class TextureChannel : uint32_t
	{
		Tex_RGB = 0,
		Tex_RGBA,

		Count
	};

	enum VertexMask : uint32_t
	{
		Position = 0,
		Normal,
		Color,
		UV
	};

	struct ContextMap
	{
		/// <summary>
		///	Texture1D
		/// Texture2D
		///	Texture3D
		///	TextureCube
		///	Texture1DA
		///	Texture2DA
		/// </summary>
		static std::array<uint32_t, (std::size_t)TextureType::Count> TextureType;

		/// <summary>
		/// Tex_REPEAT
		/// Tex_MIRRORED_REPEAT
		///	Tex_CLAMP_TO_EDGE
		///	Tex_CLAMP_TO_BORDER
		/// </summary>
		static std::array<uint32_t, (std::size_t)TextureWrap::Count> TextureWrap;

		/// <summary>
		/// Tex_NEAREST
		/// Tex_LINEAR
		/// </summary>
		static std::array<uint32_t, (std::size_t)TextureFilter::Count> TextureFilter;

		/// <summary>
		/// Tex_RGB
		/// Tex_RGBA
		/// </summary>
		static std::array<uint32_t, (std::size_t)TextureChannel::Count> TextureChannel;

		/// <summary>
		/// Points
		/// Lines
		///	Lines_Loop
		///	Lines_Strip
		///	Triangles
		///	Triangles_Strip
		///	Triangles_Fan
		/// </summary>
		static std::array<uint32_t, (std::size_t)DrawMode::Count> DrawMode;
	};
}


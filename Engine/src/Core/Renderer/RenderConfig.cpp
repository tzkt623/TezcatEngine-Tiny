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

#include "Core/Renderer/RenderConfig.h"
#include "Core/Renderer/VertexBuffer.h"

namespace tezcat::Tiny
{
	std::array<DataMemFormatWrapper, (uint32_t)DataMemFormat::Count> ContextMap::DataMemFormatArray =
	{
		DataMemFormatWrapper(DataMemFormat::Byte,		0),
		DataMemFormatWrapper(DataMemFormat::UByte,		0),
		DataMemFormatWrapper(DataMemFormat::Short,		0),
		DataMemFormatWrapper(DataMemFormat::UShort,		0),
		DataMemFormatWrapper(DataMemFormat::Int32,		0),
		DataMemFormatWrapper(DataMemFormat::UInt32,		0),
		DataMemFormatWrapper(DataMemFormat::Int64,		0),
		DataMemFormatWrapper(DataMemFormat::UInt64,		0),
		DataMemFormatWrapper(DataMemFormat::Float,		0),
		DataMemFormatWrapper(DataMemFormat::Float64,		0),
		DataMemFormatWrapper(DataMemFormat::UInt_24_8,	0),
	};

	std::array<TexTypeWrapper, (uint32_t)TextureType::Count> ContextMap::TextureTypeArray =
	{
		TexTypeWrapper(TextureType::Texture1D,			0),
		TexTypeWrapper(TextureType::Texture2D,			0),
		TexTypeWrapper(TextureType::Texture3D,			0),
		TexTypeWrapper(TextureType::TextureCube,		0),
		TexTypeWrapper(TextureType::Texture1DA,			0),
		TexTypeWrapper(TextureType::Texture2DA,			0),
		TexTypeWrapper(TextureType::TextureRender2D,	0),
	};

	std::array<TexWrapWrapper, (uint32_t)TextureWrap::Count> ContextMap::TextureWrapArray =
	{
		TexWrapWrapper(TextureWrap::Repeat,				0),
		TexWrapWrapper(TextureWrap::Mirrored_Repeat,	0),
		TexWrapWrapper(TextureWrap::Clamp_To_Edge,		0),
		TexWrapWrapper(TextureWrap::Clamp_To_Border,	0)
	};

	std::array<TexFilterWrapper, (uint32_t)TextureFilter::Count> ContextMap::TextureFilterArray =
	{
		TexFilterWrapper(TextureFilter::Nearest,				0),
		TexFilterWrapper(TextureFilter::Linear,					0),
		TexFilterWrapper(TextureFilter::Linear_Mipmap_Linear,	0),
	};

	std::array<TexInternalFormatWrapper, (uint32_t)TextureInternalFormat::Count> ContextMap::TextureInternalFormatArray =
	{
		TexInternalFormatWrapper(TextureInternalFormat::None,			0),
		TexInternalFormatWrapper(TextureInternalFormat::Depth,			0),
		TexInternalFormatWrapper(TextureInternalFormat::Depth_Stencil,	0),
		TexInternalFormatWrapper(TextureInternalFormat::R,				0),
		TexInternalFormatWrapper(TextureInternalFormat::RG,				0),
		TexInternalFormatWrapper(TextureInternalFormat::RGB,			0),
		TexInternalFormatWrapper(TextureInternalFormat::RGBA,			0),
		TexInternalFormatWrapper(TextureInternalFormat::R8,				0),
		TexInternalFormatWrapper(TextureInternalFormat::RG8,			0),
		TexInternalFormatWrapper(TextureInternalFormat::RGB8,			0),
		TexInternalFormatWrapper(TextureInternalFormat::RGBA8,			0),
		TexInternalFormatWrapper(TextureInternalFormat::R16,			0),
		TexInternalFormatWrapper(TextureInternalFormat::RG16,			0),
		TexInternalFormatWrapper(TextureInternalFormat::RGB16,			0),
		TexInternalFormatWrapper(TextureInternalFormat::RGBA16,			0),
		TexInternalFormatWrapper(TextureInternalFormat::R16F,			0),
		TexInternalFormatWrapper(TextureInternalFormat::RG16F,			0),
		TexInternalFormatWrapper(TextureInternalFormat::RGB16F,			0),
		TexInternalFormatWrapper(TextureInternalFormat::RGBA16F,		0),
		TexInternalFormatWrapper(TextureInternalFormat::R32F,			0),
		TexInternalFormatWrapper(TextureInternalFormat::RG32F,			0),
		TexInternalFormatWrapper(TextureInternalFormat::RGB32F,			0),
		TexInternalFormatWrapper(TextureInternalFormat::RGBA32F,		0),
		TexInternalFormatWrapper(TextureInternalFormat::R32UI,			0),
	};

	std::array<TexFormatWrapper, (uint32_t)TextureFormat::Count> ContextMap::TextureFormatArray =
	{
		TexFormatWrapper(TextureFormat::None,			0),
		TexFormatWrapper(TextureFormat::Depth,			0),
		TexFormatWrapper(TextureFormat::Stencil,		0),
		TexFormatWrapper(TextureFormat::Depth_Stencil,	0),
		TexFormatWrapper(TextureFormat::R,				0),
		TexFormatWrapper(TextureFormat::RG,				0),
		TexFormatWrapper(TextureFormat::RGB,			0),
		TexFormatWrapper(TextureFormat::RGBA,			0),
		TexFormatWrapper(TextureFormat::BGR,			0),
		TexFormatWrapper(TextureFormat::BGRA,			0),
		TexFormatWrapper(TextureFormat::RI,				0),

	};

	std::array<ColorBufferWrapper, (uint32_t)ColorBuffer::Count> ContextMap::ColorBufferArray =
	{
		ColorBufferWrapper(ColorBuffer::None,	0),
		ColorBufferWrapper(ColorBuffer::A0,		0),
		ColorBufferWrapper(ColorBuffer::A1,		0),
		ColorBufferWrapper(ColorBuffer::B0,		0),
		ColorBufferWrapper(ColorBuffer::B1,		0),
		ColorBufferWrapper(ColorBuffer::AX,		0),
		ColorBufferWrapper(ColorBuffer::BX,		0),
		ColorBufferWrapper(ColorBuffer::X0,		0),
		ColorBufferWrapper(ColorBuffer::X1,		0),
		ColorBufferWrapper(ColorBuffer::All,	0)
	};


	std::array<DrawModeWrapper, (uint32_t)DrawMode::Count> ContextMap::DrawModeArray =
	{
		DrawModeWrapper(DrawMode::Points,			0),
		DrawModeWrapper(DrawMode::Lines,			0),
		DrawModeWrapper(DrawMode::Lines_Loop,		0),
		DrawModeWrapper(DrawMode::Lines_Strip,		0),
		DrawModeWrapper(DrawMode::Triangles,		0),
		DrawModeWrapper(DrawMode::Triangles_Strip,	0),
		DrawModeWrapper(DrawMode::Triangles_Fan,	0)
	};

	std::array<PolygonModeWrapper, (uint32_t)PolygonMode::Count> ContextMap::PolygonModeArray =
	{
		PolygonModeWrapper(PolygonMode::Point,	0),
		PolygonModeWrapper(PolygonMode::Line,	0),
		PolygonModeWrapper(PolygonMode::Face,	0),
	};


	std::unordered_map<std::string, CullFaceWrapper> ContextMap::CullFaceMap =
	{
		{"Off",		CullFaceWrapper(CullFace::Off,		0)},
		{"Front",	CullFaceWrapper(CullFace::Front,	0)},
		{"Back",	CullFaceWrapper(CullFace::Back,		0)},
		{"All",		CullFaceWrapper(CullFace::All,		0)}
	};

	std::array<CullFaceWrapper, (uint32_t)CullFace::Count> ContextMap::CullFaceArray
	{
		CullFaceWrapper(CullFace::Off,		0),
		CullFaceWrapper(CullFace::Front,	0),
		CullFaceWrapper(CullFace::Back,		0),
		CullFaceWrapper(CullFace::All,		0)
	};

	std::unordered_map<std::string, BlendWrapper> ContextMap::BlendMap =
	{
		{"0",			BlendWrapper(Blend::Zero,					0)},
		{"1",			BlendWrapper(Blend::One,					0)},
		{"Src",			BlendWrapper(Blend::Source,					0)},
		{"1-Src",		BlendWrapper(Blend::One_Minus_Source,		0)},
		{"Tar",			BlendWrapper(Blend::Target,					0)},
		{"1-Tar",		BlendWrapper(Blend::One_Minus_Target,		0)},
		{"SrcA",		BlendWrapper(Blend::SourceAlpha,			0)},
		{"1-SrcA",		BlendWrapper(Blend::One_Minus_SourceAlpha,	0)},
		{"TarA",		BlendWrapper(Blend::TargetAlpha,			0)},
		{"1-TarA",		BlendWrapper(Blend::One_Minus_Target,		0)},
		{"Const",		BlendWrapper(Blend::Const,					0)},
		{"1-Const",		BlendWrapper(Blend::One_Minus_Const,		0)},
		{"ConstA",		BlendWrapper(Blend::ConstAlpha,				0)},
		{"1-ConstA",	BlendWrapper(Blend::One_Minus_ConstAlpha,	0)}
	};

	std::array<BlendWrapper, (uint32_t)Blend::Count> ContextMap::BlendArray =
	{
		BlendWrapper(Blend::Zero,					0),
		BlendWrapper(Blend::One,					0),
		BlendWrapper(Blend::Source,					0),
		BlendWrapper(Blend::One_Minus_Source,		0),
		BlendWrapper(Blend::Target,					0),
		BlendWrapper(Blend::One_Minus_Target,		0),
		BlendWrapper(Blend::SourceAlpha,			0),
		BlendWrapper(Blend::One_Minus_SourceAlpha,	0),
		BlendWrapper(Blend::TargetAlpha,			0),
		BlendWrapper(Blend::One_Minus_Target,		0),
		BlendWrapper(Blend::Const,					0),
		BlendWrapper(Blend::One_Minus_Const,		0),
		BlendWrapper(Blend::ConstAlpha,				0),
		BlendWrapper(Blend::One_Minus_ConstAlpha,	0)
	};


	std::unordered_map<std::string, DepthTestWrapper> ContextMap::DepthTestMap =
	{
		{"Off",				DepthTestWrapper(DepthTest::Off,			0)},
		{"Always",			DepthTestWrapper(DepthTest::Always,			0)},
		{"Never",			DepthTestWrapper(DepthTest::Never,			0)},
		{"Less",			DepthTestWrapper(DepthTest::Less,			0)},
		{"LessEqual",		DepthTestWrapper(DepthTest::LessEqual,		0)},
		{"Greater",			DepthTestWrapper(DepthTest::Greater,		0)},
		{"GreaterEqual",	DepthTestWrapper(DepthTest::GreaterEqual,	0)},
		{"Equal",			DepthTestWrapper(DepthTest::Equal,			0)},
		{"NotEqual",		DepthTestWrapper(DepthTest::NotEqual,		0)}
	};

	std::array<DepthTestWrapper, (uint32_t)DepthTest::Count> ContextMap::DepthTestArray =
	{
		DepthTestWrapper(DepthTest::Off,			0),
		DepthTestWrapper(DepthTest::Always,			0),
		DepthTestWrapper(DepthTest::Never,			0),
		DepthTestWrapper(DepthTest::Less,			0),
		DepthTestWrapper(DepthTest::LessEqual,		0),
		DepthTestWrapper(DepthTest::Greater,		0),
		DepthTestWrapper(DepthTest::GreaterEqual,	0),
		DepthTestWrapper(DepthTest::Equal,			0),
		DepthTestWrapper(DepthTest::NotEqual,		0)
	};

	std::unordered_map<std::string, LightMode> ContextMap::LightModeMap =
	{
		{"Unlit",		LightMode::Unlit },
		{"Forward",		LightMode::Forward },
		{"ForwardAdd",	LightMode::ForwardAdd },
		{"Deferred",	LightMode::Deferred },
	};

	std::unordered_map<std::string, Queue> ContextMap::QueueMap =
	{
		{"None",		Queue::None },
		{"Prepare",		Queue::Prepare },
		{"Background",	Queue::Background },
		{"Opaque",		Queue::Opaque },
		{"AlphaTest",	Queue::AlphaTest },
		{"OpaqueLast",	Queue::OpaqueLast },
		{"Transparent", Queue::Transparent },
		{"Overlay",		Queue::Overlay },
		{"Post",		Queue::Post }
	};

	std::unordered_map<std::string, UniformType> ContextMap::UniformTypeUMap
	{
		{ "bool",		UniformType::Bool },
		{ "int",		UniformType::Int },
		{ "int2",		UniformType::Int2 },
		{ "int3",		UniformType::Int3 },
		{ "int4",		UniformType::Int4 },
		{ "uint",		UniformType::UInt },
		{ "uint2",		UniformType::UInt2 },
		{ "uint3",		UniformType::UInt3 },
		{ "uint4",		UniformType::UInt4 },
		{ "float",		UniformType::Float },
		{ "float2",		UniformType::Float2 },
		{ "float3",		UniformType::Float3 },
		{ "float4",		UniformType::Float4 },
		{ "double",		UniformType::Double },
		{ "double2",	UniformType::Double2 },
		{ "double3",	UniformType::Double3 },
		{ "double4",	UniformType::Double4 },
		{ "float3x3",	UniformType::Mat3 },
		{ "float4x4",	UniformType::Mat4 },
		{ "tex2D",		UniformType::Tex2D },
		{ "tex3D",		UniformType::Tex3D },
		{ "texCube",	UniformType::TexCube }
	};


	void UniformBufferLayout::notifyHolderUpdateData()
	{
		for (auto& holder : mBufferSet)
		{
			holder->updateLayoutData();
		}
	}

}

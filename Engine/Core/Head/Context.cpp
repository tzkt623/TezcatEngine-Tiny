#include "Context.h"

namespace tezcat::Tiny::Core
{
	std::array<TexTypeWrapper, (std::size_t)TextureType::Count> ContextMap::TextureTypeArray =
	{
		TexTypeWrapper(TextureType::Texture1D,		0),
		TexTypeWrapper(TextureType::Texture2D,		0),
		TexTypeWrapper(TextureType::Texture3D,		0),
		TexTypeWrapper(TextureType::TextureCube,	0),
		TexTypeWrapper(TextureType::Texture1DA,		0),
		TexTypeWrapper(TextureType::Texture2DA,		0)
	};

	std::array<TexWrapWrapper, (std::size_t)TextureWrap::Count> ContextMap::TextureWrapArray =
	{
		TexWrapWrapper(TextureWrap::Tex_REPEAT,				0),
		TexWrapWrapper(TextureWrap::Tex_MIRRORED_REPEAT,	0),
		TexWrapWrapper(TextureWrap::Tex_CLAMP_TO_EDGE,		0),
		TexWrapWrapper(TextureWrap::Tex_CLAMP_TO_BORDER,	0)
	};

	std::array<TexFilterWrapper, (std::size_t)TextureFilter::Count> ContextMap::TextureFilterArray =
	{
		TexFilterWrapper(TextureFilter::Tex_NEAREST,	0),
		TexFilterWrapper(TextureFilter::Tex_LINEAR,		0)
	};

	std::array<TexChannelWrapper, (std::size_t)TextureChannel::Count> ContextMap::TextureChannelArray =
	{
		TexChannelWrapper(TextureChannel::Tex_RGB,	0),
		TexChannelWrapper(TextureChannel::Tex_RGBA,	0)
	};

	std::array<DrawModeWrapper, (std::size_t)DrawMode::Count> ContextMap::DrawModeArray =
	{
		DrawModeWrapper(DrawMode::Points,			0),
		DrawModeWrapper(DrawMode::Lines,			0),
		DrawModeWrapper(DrawMode::Lines_Loop,		0),
		DrawModeWrapper(DrawMode::Lines_Strip,		0),
		DrawModeWrapper(DrawMode::Triangles,		0),
		DrawModeWrapper(DrawMode::Triangles_Strip,	0),
		DrawModeWrapper(DrawMode::Triangles_Fan,	0)
	};

	std::unordered_map<std::string, CullFaceWrapper> ContextMap::CullFaceMap =
	{
		{"Off",		CullFaceWrapper(CullFace::Off,		0)},
		{"Front",	CullFaceWrapper(CullFace::Front,	0)},
		{"Back",	CullFaceWrapper(CullFace::Back,		0)},
		{"All",		CullFaceWrapper(CullFace::All,		0)}
	};

	std::array<CullFaceWrapper, (std::size_t)CullFace::Count> ContextMap::CullFaceArray
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

	std::array<BlendWrapper, (std::size_t)Blend::Count> ContextMap::BlendArray =
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
}
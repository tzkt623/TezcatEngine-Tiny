#include "RenderConfig.h"

namespace tezcat::Tiny
{
	std::array<DataTypeWrapper, (std::size_t)DataType::Count> ContextMap::DataTypeArray =
	{
		DataTypeWrapper(DataType::Byte,			0),
		DataTypeWrapper(DataType::UByte,		0),
		DataTypeWrapper(DataType::Short,		0),
		DataTypeWrapper(DataType::UShort,		0),
		DataTypeWrapper(DataType::Int32,		0),
		DataTypeWrapper(DataType::UInt32,		0),
		DataTypeWrapper(DataType::Int64,		0),
		DataTypeWrapper(DataType::UInt64,		0),
		DataTypeWrapper(DataType::Float32,		0),
		DataTypeWrapper(DataType::Float64,		0),
		DataTypeWrapper(DataType::UInt_24_8,	0),
	};

	std::array<TexTypeWrapper, (std::size_t)TextureType::Count> ContextMap::TextureTypeArray =
	{
		TexTypeWrapper(TextureType::Texture1D,			0),
		TexTypeWrapper(TextureType::Texture2D,			0),
		TexTypeWrapper(TextureType::Texture3D,			0),
		TexTypeWrapper(TextureType::TextureCube,		0),
		TexTypeWrapper(TextureType::Texture1DA,			0),
		TexTypeWrapper(TextureType::Texture2DA,			0),
		TexTypeWrapper(TextureType::TextureRender2D,	0),
	};

	std::array<TexWrapWrapper, (std::size_t)TextureWrap::Count> ContextMap::TextureWrapArray =
	{
		TexWrapWrapper(TextureWrap::Repeat,				0),
		TexWrapWrapper(TextureWrap::Mirrored_Repeat,	0),
		TexWrapWrapper(TextureWrap::Clamp_To_Edge,		0),
		TexWrapWrapper(TextureWrap::Clamp_To_Border,	0)
	};

	std::array<TexFilterWrapper, (std::size_t)TextureFilter::Count> ContextMap::TextureFilterArray =
	{
		TexFilterWrapper(TextureFilter::Nearest,	0),
		TexFilterWrapper(TextureFilter::Linear,		0)
	};

	std::array<TexChannelWrapper, (std::size_t)TextureChannel::Count> ContextMap::TextureChannelArray =
	{
		TexChannelWrapper(TextureChannel::None,					0),
		TexChannelWrapper(TextureChannel::R,					0),
		TexChannelWrapper(TextureChannel::RG,					0),
		TexChannelWrapper(TextureChannel::RGB,					0),
		TexChannelWrapper(TextureChannel::RGBA,					0),
		TexChannelWrapper(TextureChannel::Depth,				0),
		TexChannelWrapper(TextureChannel::Depth16,				0),
		TexChannelWrapper(TextureChannel::Depth24,				0),
		TexChannelWrapper(TextureChannel::Depth32,				0),
		TexChannelWrapper(TextureChannel::Depth32f,				0),
		TexChannelWrapper(TextureChannel::Depth_Stencil,		0),
		TexChannelWrapper(TextureChannel::Depth24_Stencil8,		0),
		TexChannelWrapper(TextureChannel::Depth32f_Stencil8,	0),
		TexChannelWrapper(TextureChannel::Stencil8,				0),
		TexChannelWrapper(TextureChannel::RGBF16,				0),
	};

	std::array<ColorBufferWrapper, (std::size_t)ColorBuffer::Count> ContextMap::ColorBufferArray =
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

	std::array<DepthTestWrapper, (std::size_t)DepthTest::Count> ContextMap::DepthTestArray =
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
		{"None",		Queue::None},
		{"Prepare",		Queue::Prepare},
		{"Background",	Queue::Background},
		{"Opaque",		Queue::Opaque},
		{"AlphaTest",	Queue::AlphaTest},
		{"OpaqueLast",	Queue::OpaqueLast},
		{"Transparent", Queue::Transparent},
		{"Overlay",		Queue::Overlay},
		{"Post",		Queue::Post}
	};

}

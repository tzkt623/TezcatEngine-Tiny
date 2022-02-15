#include "ContextMap.h"

namespace tezcat::Tiny::Core
{
	std::array<uint32_t, (std::size_t)TextureType::Count> ContextMap::TextureType;
	std::array<uint32_t, (std::size_t)TextureWrap::Count> ContextMap::TextureWrap;
	std::array<uint32_t, (std::size_t)TextureFilter::Count> ContextMap::TextureFilter;
	std::array<uint32_t, (std::size_t)TextureChannel::Count> ContextMap::TextureChannel;
	std::array<uint32_t, (std::size_t)DrawMode::Count> ContextMap::DrawMode;

}




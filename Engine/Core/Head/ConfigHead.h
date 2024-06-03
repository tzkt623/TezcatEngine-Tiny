#pragma once

#ifdef TINY_PLATFORM_WINDOWS

#ifdef TINY_DYNAMIC_LIB
#define TINY_API __declspec(dllexport)
#elif TINY_STATIC_LIB
#define TINY_API
#else 
#define TINY_API __declspec(dllimport)
#endif


#else
#error Windows Only
#endif

#define interface public virtual


namespace tezcat::Tiny
{
	enum class TINY_API RenderAPI
	{
		Error = 0,
		OpenGL,
		D3D,
		Vulkan
	};

	class TINY_API EngineConfig
	{
		friend class EngineIniter;
	public:
		static RenderAPI getRenderAPI() { return sRenderAPI; }

	private:
		static RenderAPI sRenderAPI;
	};
}

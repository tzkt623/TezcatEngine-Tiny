#pragma once

namespace tezcat::Tiny::Core { class Engine; }

namespace tezcat::Tiny::Module
{
	class ResourceLoader
	{
	public:
		virtual void prepareResource(Core::Engine* engine);
		virtual void prepareEngine(Core::Engine* engine);
		virtual void prepareScene(Core::Engine* engine);
	};
}
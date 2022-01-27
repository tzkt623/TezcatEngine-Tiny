#pragma once
#include <string>

namespace tezcat::Tiny::Core { class Engine; }

namespace tezcat::Tiny::Module
{
	class ResourceLoader
	{
	public:
		~ResourceLoader();

		/// <summary>
		/// 引擎初始化之前调用
		/// 调用顺序1
		/// </summary>
		virtual void prepareEngine(Core::Engine* engine);

		/// <summary>
		/// 加载资源
		/// 调用顺序2
		/// </summary>
		virtual void prepareResource(Core::Engine* engine);

		/// <summary>
		/// 准备Scene
		/// 调用顺序3
		/// </summary>
		virtual void prepareScene(Core::Engine* engine);

	public:
		int getWindowHeight() const { return m_WindowHeight; }
		int getWindowWidth() const { return m_WindowWidth; }

		std::string getName()
		{
			return std::string(m_GameName.begin(), m_GameName.end());
		}

	protected:
		int m_WindowWidth;
		int m_WindowHeight;
		std::u8string m_GameName;
	};
}
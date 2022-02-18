#pragma once
#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"

namespace tezcat::Tiny::Core
{
	class Engine;
	class TINY_API ResourceLoader
	{
	public:
		ResourceLoader(RenderAPI renderAPI);
		virtual ~ResourceLoader();

		/// <summary>
		/// �����ʼ��֮ǰ����
		/// ����˳��1
		/// </summary>
		virtual void prepareEngine(Engine* engine);

		/// <summary>
		/// ������Դ
		/// ����˳��2
		/// </summary>
		virtual void prepareResource(Engine* engine);

		/// <summary>
		/// ׼��Scene
		/// ����˳��3
		/// </summary>
		virtual void prepareScene(Engine* engine);

		virtual void initYourShaderParam() {}

	public:
		int getWindowHeight() const { return m_WindowHeight; }
		int getWindowWidth() const { return m_WindowWidth; }

		const std::string& getName()
		{
			static std::string name(m_GameName.begin(), m_GameName.end());
			return name;
		}

	protected:
		int m_WindowWidth;
		int m_WindowHeight;

		std::u8string m_GameName;
	};
}
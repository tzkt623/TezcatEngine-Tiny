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
		/// 执行顺序1
		/// 初始化引擎
		/// </summary>
		virtual void prepareEngine(Engine* engine);

		/// <summary>
		/// 加载资源
		/// 执行顺序2
		/// </summary>
		virtual void prepareResource(Engine* engine);

		/// <summary>
		/// 加载场景
		/// 执行顺序3
		/// </summary>
		virtual void prepareGame(Engine* engine);
		virtual void initYourShaderParam() {}

	public:
		int getWindowHeight() const { return mWindowHeight; }
		int getWindowWidth() const { return mWindowWidth; }

		const std::string& getName() const
		{
			static std::string name(mGameName.begin(), mGameName.end());
			return name;
		}

		const std::string& getResourceFolderName() const
		{
			return mResourceFolderName;
		}

	private:
		void createSomeMode();
		void createCube();
		void createSphere();
		void createSphere(int prec);
		void createSquare();
		void createSkybox();

	protected:
		int mWindowWidth;
		int mWindowHeight;

		std::u8string mGameName;
		std::string mResourceFolderName;
	};
}

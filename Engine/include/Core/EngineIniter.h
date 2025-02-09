#pragma once

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

#include "Head/CppHead.h"
#include "Head/ConfigHead.h"

namespace tezcat::Tiny
{
	class Engine;
	class TINY_API EngineIniter
	{
	public:
		EngineIniter(RenderAPI renderAPI);
		virtual ~EngineIniter();

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
		bool isEnabelVsync() const { return mEnableVsync; }

		void setGLVersion(int major, int minor)
		{
			mGLMinor = minor;
			mGLMajor = major;
		}

		auto getGLVersion()
		{
			return std::tuple(mGLMinor, mGLMajor);
		}

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
		void createBuildinModules();
		void createCube();
		void createSphere();
		void createSphere(int prec);
		void createSquare();
		void createSkybox();
		void createPlane();
		void createGridSquare();
		void createSphere2(uint32_t X_SEGMENTS, uint32_t Y_SEGMENTS);
		void createRect();
	protected:
		int mWindowWidth;
		int mWindowHeight;

		std::u8string mGameName;
		std::string mResourceFolderName;

		bool mEnableVsync;

		int mGLMajor;
		int mGLMinor;
	};
}



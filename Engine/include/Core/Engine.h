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

#include "Head/ConfigHead.h"
#include "Head/TinyCpp.h"

namespace tezcat::Tiny
{
	class InputSystem;
	class EngineIniter;
	class BaseGraphics;
	class Pipeline;

	class TINY_API Engine
	{
	public:
		Engine();
		virtual ~Engine();

	public:
		bool init(EngineIniter* loader);
		void run();
		BaseGraphics* getGraphics() { return mGraphics; }
		EngineIniter* getResourceLoader() { return mResourceLoader; }
		void setEnableMultiThread()
		{
			sMultiThread = true;
		}

	protected:
		virtual bool preInit();
		virtual bool onInit();
		virtual bool postInit();

		virtual void stop();
		virtual void beforeLoop();
		virtual void endLoop();

		virtual void preUpdate();
		virtual void postUpdate();
		virtual void onUpdate();
		virtual void onRender();
		virtual void notifyRender();
		virtual void notifyRenderThread();

		virtual BaseGraphics* createGraphics() = 0;

	protected:
		InputSystem* mInputSystem;
		EngineIniter* mResourceLoader;
		BaseGraphics* mGraphics;
		Pipeline* mPipeline;

	protected:
		bool mIsRunning;
		std::thread mRenderThread;
		std::condition_variable mThreadCondition;
		std::atomic<bool> mRenderThreadInited;

	public:
		static const int32& getScreenWidth() { return ScreenWidth; }
		static const int32& getScreenHeight() { return ScreenHeight; }
		static const float& getDeltaTime() { return sDeltaTime; }
		static const std::string& getName() { return sName; }

	protected:
		static std::string sName;
		static int32 ScreenWidth;
		static int32 ScreenHeight;
		static float sDeltaTime;

	public:
		static bool sMultiThread;

	private:
		static std::atomic<bool> allowRender;
		static std::atomic<bool> allowLogic;
		static std::mutex sMainMutex;
		static std::condition_variable sCVRenderThread;
		static std::condition_variable sCVLogicThread;
		static std::binary_semaphore sSemRenderThread;
		static std::binary_semaphore sSemLogicThread;
	};
}

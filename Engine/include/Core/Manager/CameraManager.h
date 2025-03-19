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
#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"
#include "../Base/TinyObject.h"

namespace tezcat::Tiny
{
	class Camera;
	class BaseRenderObserver;

	class TINY_API CameraData : public TinyObject
	{
		friend class CameraManager;
		CameraData();
		TINY_OBJECT_H(CameraData, TinyObject)

	public:
		Camera* getMainCamera();
		Camera* findCamera(const std::string& name);

		void setMain(Camera* camera);
		void setMain(const std::string& name);

		void addCamera(Camera* camera);
		void addRenderObserver(BaseRenderObserver* observer);

		void calculate();

	private:
		bool mDirty;
		Camera* mMain;
		std::unordered_map<std::string_view, Camera*> mCameraWithName;
		std::vector<TinyWeakRef<BaseRenderObserver>> mObserverArray;
	};

	class TINY_API CameraManager
	{
		CameraManager();
		~CameraManager() = delete;
	public:
		static void setData(CameraData* data) { mCurrentData = data; }
		static TinyWeakRef<CameraData> getData() { return mCurrentData; }

		static void setMainCamera(Camera* camera);
		static void setMainCamera(const std::string& name);
		static Camera* getMainCamera() { return mCurrentData->getMainCamera(); }
		static bool isDataValied() { return mCurrentData != nullptr; }

		static Camera* findCamera(const std::string& name);
		static void addCamera(Camera* camera);

	public:
		static void addRenderObserver(BaseRenderObserver* renderObserver);
		static void calculate();

	private:
		static CameraData* mCurrentData;
	};
}

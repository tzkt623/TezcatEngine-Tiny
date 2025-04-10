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
#include "../Head/TinyCpp.h"
#include "../Base/TinyObject.h"
#include "../Component/Camera.h"

namespace tezcat::Tiny
{
	class BaseRenderObserver;
	class CameraObserver;
	class PipelineQueue;

	class TINY_API CameraData : public TinyObject
	{
		friend class CameraManager;
		CameraData();
		TINY_OBJECT_H(CameraData, TinyObject)

	public:
		void preRender();

		const TinyWeakRef<Camera> &getMainCamera();
		Camera* findCamera(const std::string& name);

		void setMain(Camera* camera);
		void setMain(const std::string& name);

		void addCamera(Camera* camera);
		void removeCamera(Camera* camera);
	private:
		bool mDirty;
		TinyWeakRef<Camera> mMain;
		std::unordered_map<std::string_view, Camera*> mCameraWithName;
		std::vector<Camera*> mObserverArray;
		std::vector<TinyWeakRef<CameraObserver>> mRuningArray;

		std::queue<TinyUID> mFreeIDs;
	};

	class TINY_API CameraManager
	{
		CameraManager();
		~CameraManager() = delete;
	public:
		static void setData(CameraData* data) { mCurrentData = data; }
		//static CameraData* getData() { return mCurrentData; }

		static void setMainCamera(Camera* camera);
		static void setMainCamera(const std::string& name);

		static TinyWeakRef<Camera> getMainCamera() { return mCurrentData->mMain; }

		static bool isDataValied() { return mCurrentData != nullptr; }

		static Camera* findCamera(const std::string& name);
		static void addCamera(Camera* camera);
		static void removeCamera(Camera* camera);

		static bool isMain(const Camera* camera);
		static void setEditorObserver(RenderObserver* observer) { mEditorObserver = observer; }

	public:
		static void preRender();

	private:
		static CameraData* mCurrentData;
		static RenderObserver* mEditorObserver;
	};
}

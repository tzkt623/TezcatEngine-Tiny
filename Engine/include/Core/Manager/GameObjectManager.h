#pragma once
/*
	Copyright (C) 2022 - 2025 Tezcat(特兹卡特) tzkt623@qq.com

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


namespace tezcat::Tiny
{
	class GameObject;
	class ReplacedPipelinePass;
	class RenderObserver;
	class FrameBuffer;
	class BaseRenderObserver;

	class GameObjectManager
	{
	public:
		static void init();
		static void preRender();

		static int32_t addGameObject(GameObject* gameObject);
		static void removeGameObject(GameObject* gameObject);
		static void enterScene();
		static void exitScene();
		static void active();
		static void deactive();
		static bool allowPickObject();

		static const std::vector<GameObject*>& getAllObject() { return mArray; }

		static const size_t getFreeCount() noexcept { return mFreeIDQueue.size(); }
		static const size_t getSlotCount() noexcept { return mArray.size(); }
		static const size_t getObjectCount() noexcept { return mArray.size() - mFreeIDQueue.size(); }
		static GameObject* getObject(size_t index) noexcept
		{
			if (index <= 0 || index >= mArray.size())
			{
				return nullptr;
			}

			return mArray[index];
		}
		static void setIDObserver(RenderObserver* observer);

	private:
		static void createPass(BaseRenderObserver* observer);
		static void createFrameBuffer();

	private:
		static std::vector<GameObject*> mArray;
		static std::queue<int32_t> mFreeIDQueue;

		static ReplacedPipelinePass* mPass;
		static RenderObserver* mObserver;
		static FrameBuffer* mFrameBuffer;
	};
}

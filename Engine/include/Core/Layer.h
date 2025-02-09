#pragma once

#include "Head/CppHead.h"

namespace tezcat::Tiny
{
	class GameObject;
	class Camera;
	class BaseRenderer;
	class Layer
	{
	public:
		Layer();
		~Layer();

		void addRenderObejct(BaseRenderer* gameObject);
		void cullGameObject(Camera* camera);

	private:
		int mIndex;
		std::list<BaseRenderer*> mRenderObejctList;
	};
}

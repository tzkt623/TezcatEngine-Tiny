#pragma once

#include "../Head/CppHead.h"

namespace tezcat::Tiny
{
	class GameObject;
	class Camera;
	class IRenderObject;
	class Layer
	{
	public:
		Layer();
		~Layer();

		void addRenderObejct(IRenderObject* gameObject);
		void cullGameObject(Camera* camera);

	private:
		int mIndex;
		std::list<IRenderObject*> mRenderObejctList;
	};
}

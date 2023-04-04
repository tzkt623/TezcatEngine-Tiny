#pragma once

#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"

namespace tezcat::Tiny::Core
{
	class Camera;
	class PointLight;
	class IRenderObject;
	class LightLayer;
	class ILight;
	class TINY_API RenderLayer
	{
	public:
		RenderLayer();
		~RenderLayer();
	public:
		void addRenderObejct(IRenderObject* gameObject);
		void testWithCamera(Camera* camera);
		void testVisibleObjects(ILight* light);

	private:
		int mIndex;
		std::list<IRenderObject*> mRenderObjectList;
		std::list<IRenderObject*> mVisibleRenderObjectList;
	};
}

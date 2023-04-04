#pragma once

#include "../Head/CppHead.h"

namespace tezcat::Tiny::Core
{
	class ILight;
	class Pipeline;
	class IRenderObject;
	class PointLight;
	class SpotLight;
	class LightLayer
	{
	public:
		LightLayer();
		~LightLayer();

		void addLight(PointLight* light);
		void addLight(SpotLight* light);
		void addLight(ILight* light);
		bool testGameObject(Pipeline* pipeline, IRenderObject* renderObject);

	private:
		std::list<ILight *> m_LightList;
	};
}

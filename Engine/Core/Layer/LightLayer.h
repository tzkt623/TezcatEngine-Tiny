#pragma once

#include "../Head/CppHead.h"

namespace tezcat::Tiny
{
	class ILight;
	class Pipeline;
	class BaseRenderer;
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
		bool testGameObject(Pipeline* pipeline, BaseRenderer* renderObject);

	private:
		std::list<ILight *> mLightList;
	};
}

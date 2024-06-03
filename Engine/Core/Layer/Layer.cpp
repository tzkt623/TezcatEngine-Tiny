#include "Layer.h"

#include "../Component/GameObject.h"
#include "../Component/MeshRenderer.h"
#include "../Component/Camera.h"
#include "../Renderer/Renderer.h"



namespace tezcat::Tiny
{
	Layer::Layer()
		: mIndex(-1)
	{

	}

	Layer::~Layer()
	{

	}

	void Layer::addRenderObejct(BaseRenderer* renderObject)
	{
		mRenderObejctList.emplace_back(renderObject);
	}

	void Layer::cullGameObject(Camera* camera)
	{

	}
}

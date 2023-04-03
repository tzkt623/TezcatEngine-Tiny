#include "Layer.h"

#include "../Component/GameObject.h"
#include "../Component/MeshRenderer.h"
#include "../Component/Camera.h"
#include "../Renderer/RenderObject.h"



namespace tezcat::Tiny::Core
{
	Layer::Layer()
		: mIndex(-1)
	{

	}

	Layer::~Layer()
	{

	}

	void Layer::addRenderObejct(IRenderObejct* renderObject)
	{
		m_RenderObejctList.emplace_back(renderObject);
	}

	void Layer::cullGameObject(Camera* camera)
	{

	}
}

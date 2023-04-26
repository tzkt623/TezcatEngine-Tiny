#include "Layer.h"

#include "../Component/GameObject.h"
#include "../Component/MeshRenderer.h"
#include "../Component/Camera.h"
#include "../Renderer/RenderObject.h"



namespace tezcat::Tiny
{
	Layer::Layer()
		: mIndex(-1)
	{

	}

	Layer::~Layer()
	{

	}

	void Layer::addRenderObejct(IRenderObject* renderObject)
	{
		m_RenderObejctList.emplace_back(renderObject);
	}

	void Layer::cullGameObject(Camera* camera)
	{

	}
}

#include "Layer.h"

#include "GameObject.h"
#include "../Component/MeshRenderer.h"
#include "../Component/Camera.h"
#include "../Component/RenderObject.h"



namespace tezcat::Tiny::Core
{
	Layer::Layer()
		: m_Index(-1)
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
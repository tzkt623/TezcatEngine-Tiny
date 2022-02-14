#include "RenderLayer.h"
#include "../Component/RenderObject.h"
#include "../Component/Component.h"
#include "../Component/Camera.h"

namespace tezcat::Tiny::Core
{
	RenderLayer::RenderLayer()
		: m_Index(-1)
	{

	}

	RenderLayer::~RenderLayer()
	{

	}

	void RenderLayer::addRenderObejct(IRenderObejct* renderObject)
	{
		m_RenderObjectList.emplace_back(renderObject);
	}

	void RenderLayer::cullGameObject(Camera* camera)
	{
		if (m_RenderObjectList.empty())
		{
			return;
		}

		auto it = m_RenderObjectList.begin();
		while (it != m_RenderObjectList.end())
		{
			auto com = dynamic_cast<Component*>(it->get());
			if (com->isEnable())
			{
				if (camera->cullGameObject(com->getGameObject()))
				{
					it->get()->sendDataToGPU();
				}
				it++;
			}
			else
			{
				it = m_RenderObjectList.erase(it);
			}
		}
	}
}
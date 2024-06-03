#include "MeshRenderer.h"
#include "Transform.h"
#include "GameObject.h"

#include "../Renderer/RenderObjectCache.h"


namespace tezcat::Tiny
{
	TINY_OBJECT_CPP(MeshRenderer, ComponentT<MeshRenderer>)

	MeshRenderer::MeshRenderer()
		: mRenderAgent(nullptr)
	{
		mRenderAgent = MeshRenderAgent::create();
		mRenderAgent->saveObject();
	}

	MeshRenderer::~MeshRenderer()
	{
		mRenderAgent->deleteObject();
	}

	void MeshRenderer::onEnable()
	{
		if (auto transform = mGameObject->getComponent<Transform>())
		{
			mRenderAgent->setTransform(transform);
		}
	}

	void MeshRenderer::onDisable()
	{

	}

	void MeshRenderer::onStart()
	{
		RenderObjectCache::addRenderAgent(this->getGameObject()->getLayerIndex(), mRenderAgent);
	}

	void MeshRenderer::onComponentAdded(Component* component)
	{
		if (component->typeIDEqual<Transform>())
		{
			mRenderAgent->setTransform((Transform*)component);
		}
	}

	void MeshRenderer::onComponentRemoved(Component* component)
	{

	}

}

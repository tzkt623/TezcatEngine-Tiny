#pragma once

#include "../Base/TinyObject.h"
#include "../Head/RenderConfig.h"
#include "Vertex.h"

namespace tezcat::Tiny
{
	class IRenderObject;
	class Component;
	class Shader;
	class Material;
	class RenderQueue;

	/*
	* RenderAgent用于协助pipeline对渲染信息进行收集
	* 每一个RenderAgent都会在RenderObject执行OnStart时加入自己对应的RenderLayer进行缓存
	*/
	class RenderAgent : public TinyObject
	{
		RenderAgent(IRenderObject* object);
		TINY_Factory(RenderAgent);
		TINY_RTTI_H(RenderAgent);

	public:
		virtual ~RenderAgent();

		IRenderObject* getRenderObject() { return mRenderObject; }
		Material* getMaterial() { return mMaterial; }
		Component* getComponent() { return mComponent; }
		void setComponent(Component* component);

		void setVertex(Vertex* mVertex);
		Vertex* getVertex() { return mVertex; }
		void submitModelMatrix(Shader* shader);
		void sendToQueue(const RenderPhase& phase, RenderQueue* queue);

	private:
		Vertex* mVertex;
		IRenderObject* mRenderObject;
		Component* mComponent;
		Material* mMaterial;
	};
}

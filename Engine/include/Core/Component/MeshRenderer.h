#pragma once

#include "../Head/RenderConfig.h"
#include "../Head/ConfigHead.h"
#include "../Renderer/MeshRenderAgent.h"


namespace tezcat::Tiny
{
	class Pipeline;
	class Camera;
	class Vertex;
	class Shader;
	class Material;
	class MeshData;
	class RenderAgent;

	class TINY_API MeshRenderer : public ComponentT<MeshRenderer>
	{
		MeshRenderer();
		TINY_OBJECT_H(MeshRenderer, ComponentT<MeshRenderer>)
		TINY_MESH_RENDER_FUNCTION(mRenderAgent)

	public:
		virtual ~MeshRenderer();

		virtual void onComponentAdded(Component* component) override;
		virtual void onComponentRemoved(Component* component) override;

	protected:
		void onEnable() override;
		void onDisable() override;
		void onStart() override;

	private:
		MeshRenderAgent* mRenderAgent;
	};
}

#pragma once
#include "../Head/ConfigHead.h"

#include "../Renderer/RenderConfig.h"
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

	class TINY_API MeshRenderer : public ComponentAutoID<MeshRenderer>
	{
		MeshRenderer();
		TINY_OBJECT_H(MeshRenderer, ComponentAutoID<MeshRenderer>)
		TINY_MESH_RENDER_FUNCTION(mRenderAgent)

	public:
		virtual ~MeshRenderer();

		virtual void onComponentAdded(Component* component) override;
		virtual void onComponentRemoved(Component* component) override;

		MeshRenderAgent* getRenderAgent() { return mRenderAgent; }

	protected:
		void onEnable() override;
		void onDisable() override;
		void onStart() override;

	private:
		MeshRenderAgent* mRenderAgent;
	};
}

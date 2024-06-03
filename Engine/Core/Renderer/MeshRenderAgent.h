#pragma once

#include "Renderer.h"

namespace tezcat::Tiny
{
	class MeshRenderAgent : public BaseMeshRenderer
	{
		MeshRenderAgent();
		TINY_OBJECT_H(MeshRenderAgent, BaseMeshRenderer)

	public:
		virtual ~MeshRenderAgent();

	private:

	};
}

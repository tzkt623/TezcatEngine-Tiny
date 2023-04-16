#pragma once

#include "../Head/Context.h"

namespace tezcat::Tiny::Core
{
	class MeshData;

	/// <summary>
	/// 
	/// </summary>
	class RenderCommand
	{
	public:
		RenderCommand();
		virtual ~RenderCommand();

		const DrawModeWrapper& getDrawMode() { return mDrawMode; }
		void setDrawMode(DrawMode val);

	private:
		MeshData* mMeshData;
		DrawModeWrapper mDrawMode;
	};
}


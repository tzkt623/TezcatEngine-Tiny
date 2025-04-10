#pragma once

#include "include/Tiny.h"

namespace tezcat::Editor
{
	class EditorCamera : public RenderObserver
	{
		TINY_OBJECT_H(EditorCamera, RenderObserver)
	protected:
		EditorCamera();
		void init() override;
	public:
		virtual ~EditorCamera();
		void preRender() override;

	};
}

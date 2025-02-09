#pragma once

#include "RenderObserver.h"

namespace tezcat::Tiny
{
	class CameraAgent : public RenderObserver
	{
		CameraAgent();
		TINY_OBJECT_H(CameraAgent, RenderObserver)
	public:
		virtual ~CameraAgent();

	private:

	};
}

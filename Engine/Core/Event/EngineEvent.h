#pragma once

#include "../Tool/Tool.h"

namespace tezcat::Tiny
{
	class EngineEvent
	{
		EngineEvent() = delete;
		~EngineEvent() = delete;

	public:
		static Event* get() { return mEvent; }

	private:
		static Event* mEvent;
	};

	enum EngineEventID : EventID
	{
		EE_LoadScene = 0,
		EE_PopScene,
		EE_PushScene,
		EE_OnPopScene,
		EE_OnPushScene,

		EE_SetMainCamera,
		EE_ActiveSkybox,
		EE_ChangeEnvLightingImage,
		EE_ChangeEnvLightingTexture,

		EE_IDCount
	};
}

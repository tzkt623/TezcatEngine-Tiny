#pragma once

#include "Utility/Event.h"

using namespace tezcat::Tiny::Utility;
namespace tezcat::Tiny::Core
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
		EE_IDCount
	};
}

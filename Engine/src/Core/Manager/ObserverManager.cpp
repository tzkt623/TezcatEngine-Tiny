#include "Core/Manager/ObserverManager.h"


namespace tezcat::Tiny
{
	TinyUID ObserverManager::sUIDGenerator = 0;
	std::queue<tezcat::Tiny::TinyUID> ObserverManager::mFreeIDs;

	TinyUID ObserverManager::giveID()
	{
		if (!mFreeIDs.empty())
		{
			auto id = mFreeIDs.front();
			mFreeIDs.pop();
			return id;
		}
		else
		{
			return sUIDGenerator++;
		}
	}
}

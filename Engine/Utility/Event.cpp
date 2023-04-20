#include "Event.h"

namespace tezcat::Tiny::Utility
{
	void Event::addListener(const EventID& eventID, void* master, const std::function<void(const EventData&)>& function)
	{
		auto root = mList[eventID];

		auto listener = new Listener();
		listener->callback = function;
		root->push(listener);

		auto result2 = mListenerWithOwnerUMap.find(master);
		if (result2 == mListenerWithOwnerUMap.end())
		{
			auto vector = new std::vector<Listener*>();
			mListenerWithOwnerUMap.emplace(master, vector);
			vector->push_back(listener);
		}
		else
		{
			result2->second->emplace_back(listener);
		}
	}

	bool Event::removeListener(void* master)
	{
		auto it = mListenerWithOwnerUMap.find(master);
		if (it != mListenerWithOwnerUMap.end())
		{
			auto vector = it->second;
			for (auto l : *vector)
			{
				l->removeSelf();
			}

			delete vector;
			mListenerWithOwnerUMap.erase(it);
			return true;
		}

		return false;
	}

	void Event::dispatch(const EventData& eventData)
	{
		auto list = mList[eventData.eventID];
		auto root = list->mRoot;
		while (root != nullptr)
		{
			root->callback(eventData);
			root = root->nextData;
		}
	}

	void Event::init(int eventCount)
	{
		mList.resize(eventCount, new ListenerList());
	}

}

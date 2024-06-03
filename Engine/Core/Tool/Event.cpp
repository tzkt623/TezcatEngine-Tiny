#include "Event.h"

namespace tezcat::Tiny
{
	void Event::addListener(const EventID& eventID, void* master, const std::function<void(const EventData&)>& function)
	{
		auto& root = mListenerList[eventID];
		if (!root)
		{
			root = TinyUnique<ListenerList>();
		}

		auto listener = TinyShared<Listener>();
		listener->callback = function;
		root->push(listener);

		auto result2 = mListenerWithOwnerUMap.find(master);
		if (result2 == mListenerWithOwnerUMap.end())
		{
			auto vector = TinyUnique<std::vector<std::shared_ptr<Listener>>>();
			vector->emplace_back(listener);
			mListenerWithOwnerUMap.emplace(master, std::move(vector));
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
			auto& vector = it->second;
			for (auto& l : *vector)
			{
				l->removeSelf();
			}

			mListenerWithOwnerUMap.erase(it);
			return true;
		}

		return false;
	}

	void Event::dispatch(const EventData& eventData)
	{
		auto& ptr = mListenerList[eventData.eventID];
		if (ptr)
		{
			auto root = ptr->mRoot;
			while (root)
			{
				root->callback(eventData);
				root = root->nextData;
			}
		}
	}

	void Event::init(int eventCount)
	{
		//mListenerList.resize(eventCount, TinyMove(TinyUnique<ListenerList>()));
		mListenerList.resize(eventCount);

		for (int i = 0; i < eventCount; i++)
		{

		}
	}

	Event::~Event()
	{
		mListenerWithOwnerUMap.clear();
		mListenerList.clear();
	}

}

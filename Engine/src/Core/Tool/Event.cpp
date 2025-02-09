/*
	Copyright (C) 2024 Tezcat(特兹卡特) tzkt623@qq.com

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "Core/Tool/Event.h"

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

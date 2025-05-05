/*
	Copyright (C) 2022 - 2025 Tezcat(特兹卡特) tzkt623@qq.com

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
	std::queue<Event::Listener*> Event::mListenerPool;

	Event::Event()
	{

	}

	Event::~Event()
	{
		for (auto list : mListenerListArray)
		{
			delete list;
		}

		mListenerWithOwnerUMap.clear();
		mListenerListArray.clear();
	}

	void Event::init(int eventCount)
	{
		mListenerListArray.resize(eventCount, nullptr);

		for (int i = 0; i < eventCount; i++)
		{
			mListenerListArray[i] = new ListenerList();
		}
	}


	void Event::addListener(const EventID& eventID, void* master, const std::function<void(const EventData&)>& function)
	{
		auto& root = mListenerListArray[eventID];
		auto listener = createLisenter();
		listener->callback = function;
		root->push(listener);

		auto result2 = mListenerWithOwnerUMap.find(master);
		if (result2 == mListenerWithOwnerUMap.end())
		{
			std::vector<Listener*> vector;
			vector.emplace_back(listener);
			mListenerWithOwnerUMap.emplace(master, std::move(vector));
		}
		else
		{
			result2->second.emplace_back(listener);
		}
	}

	bool Event::removeListener(void* master)
	{
		auto it = mListenerWithOwnerUMap.find(master);
		if (it != mListenerWithOwnerUMap.end())
		{
			auto& vector = it->second;
			for (auto& l : vector)
			{
				l->removeSelf();
				mListenerPool.push(l);
			}
			vector.clear();

			mListenerWithOwnerUMap.erase(it);
			return true;
		}

		return false;
	}

	void Event::dispatch(const EventData& eventData)
	{
		auto& ptr = mListenerListArray[eventData.eventID];
		auto root = ptr->mRoot;
		while (root)
		{
			root->callback(eventData);
			root = root->nextData;
		}
	}

	Event::Listener* Event::createLisenter()
	{
		if (!mListenerPool.empty())
		{
			auto listener = mListenerPool.front();
			mListenerPool.pop();
			return listener;
		}

		return new Event::Listener();
	}


	void Event::Listener::removeSelf()
	{
		if (this->preData)
		{
			//------------------------------------------
			//	删除的不是第一个
			//	如果后一个有值
			//	先设置后一个的前一个
			if (this->nextData)
			{
				this->nextData->preData = this->preData;
			}
			this->preData->nextData = this->nextData;
		}
		else
		{
			//-----------------------
			//	删除的是第一个
			if (this->nextData)
			{
				this->nextData->preData = nullptr;
				//如果前面没有值,说明它是第一个
				//把下一个装到头上来
				list->mRoot = this->nextData;
			}
			else
			{
				list->mRoot = nullptr;
			}
		}

		this->preData = nullptr;
		this->nextData = nullptr;
		this->list = nullptr;
		mListenerPool.push(this);
	}

	Event::Listener::~Listener()
	{
		this->preData = nullptr;
		this->nextData = nullptr;
		this->list = nullptr;
	}

	Event::Listener::Listener()
		: preData(nullptr)
		, nextData(nullptr)
		, list(nullptr)
	{

	}

	Event::ListenerList::~ListenerList()
	{
		while (mRoot)
		{
			auto next = mRoot->nextData;
			delete mRoot;
			mRoot = next;
		}
	}

	Event::ListenerList::ListenerList() : mRoot(nullptr)
	{

	}

	Event::ListenerList::ListenerList(const ListenerList& other) : mRoot(other.mRoot)
	{

	}

	void Event::ListenerList::push(Listener* listener)
	{
		listener->list = this;

		if (mRoot)
		{
			mRoot->preData = listener;
			listener->nextData = mRoot;
			mRoot = listener;
		}
		else
		{
			mRoot = listener;
		}
	}

}

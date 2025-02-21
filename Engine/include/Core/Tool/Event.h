#pragma once
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
#include "../Head/ConfigHead.h"
#include "../Head/CppHead.h"


namespace tezcat::Tiny
{
	typedef uint32 EventID;

	struct EventData
	{
		EventID eventID;
		void* userData;
	};

	class TINY_API Event
	{
		class ListenerList;
		class Listener
		{
		public:
			Listener()
			{

			}
			~Listener()
			{
				this->preData = nullptr;
				this->list = nullptr;
			}


			void removeSelf()
			{
				if (this->preData)
				{
					//删除的不是第一个
					// 
					//如果后一个有值
					//先设置后一个的前一个
					if (this->nextData)
					{
						this->nextData->preData = this->preData;
					}
					this->preData->nextData = std::move(this->nextData);
				}
				else
				{
					//删除的是第一个
					if (this->nextData)
					{
						this->nextData->preData = nullptr;
						//如果前面没有值,说明它是第一个
						//把下一个装到头上来
						list->mRoot = std::move(this->nextData);
					}
					else
					{
						list->mRoot.reset();
					}
				}
			}

			std::function<void(const EventData&)> callback;
			ListenerList* list = nullptr;
			Listener* preData = nullptr;
			std::shared_ptr<Listener> nextData;
		};

		class ListenerList
		{
		public:
			ListenerList()
			{

			}

			~ListenerList()
			{
				while (mRoot)
				{
					auto next = std::move(mRoot->nextData);
					mRoot = std::move(next);
				}
			}

			ListenerList(const ListenerList& other)
				: mRoot(other.mRoot)
			{

			}

		public:
			void push(std::shared_ptr<Listener>& listener)
			{
				listener->list = this;

				if (mRoot)
				{
					mRoot->preData = listener.get();
					listener->nextData = std::move(mRoot);
					mRoot = std::move(listener);
				}
				else
				{
					mRoot = std::move(listener);
				}
			}

			std::shared_ptr<Listener> mRoot;
		};

	public:
		Event() {}
		~Event();

	public:
		void init(int eventCount);

	public:
		void addListener(const EventID& eventID, void* master, const std::function<void(const EventData&)>& function);
		bool removeListener(void* master);
		void dispatch(const EventData& eventData);

	private:
		std::vector<std::unique_ptr<ListenerList>> mListenerList;
		std::unordered_map<void*, std::unique_ptr<std::vector<std::shared_ptr<Listener>>>> mListenerWithOwnerUMap;
	};
}

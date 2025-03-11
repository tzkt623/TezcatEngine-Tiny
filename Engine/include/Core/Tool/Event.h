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
	typedef uint32_t EventID;

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
			Listener();
			~Listener();

			void removeSelf();

			std::function<void(const EventData&)> callback;
			ListenerList* list = nullptr;
			Listener* preData = nullptr;
			Listener* nextData = nullptr;
		};

		class ListenerList
		{
		public:
			ListenerList();
			~ListenerList();

			ListenerList(const ListenerList& other);

		public:
			void push(Listener* listener);

			Listener* mRoot;
		};

	public:
		Event();
		~Event();

	public:
		void init(int eventCount);

	public:
		void addListener(const EventID& eventID, void* master, const std::function<void(const EventData&)>& function);
		bool removeListener(void* master);
		void dispatch(const EventData& eventData);

	private:

	private:
		std::vector<ListenerList*> mListenerListArray;
		std::unordered_map<void*, std::vector<Listener*>> mListenerWithOwnerUMap;

	private:
		static Listener* createLisenter();
		static std::queue<Listener*> mListenerPool;
	};
}

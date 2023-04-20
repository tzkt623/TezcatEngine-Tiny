#pragma once
#include "Core/Head/ConfigHead.h"
#include "Core/Head/CppHead.h"


namespace tezcat::Tiny::Utility
{
#define BindEvent(x) std::bind(&x, this, std::placeholders::_1)

	typedef uint32_t EventID;

	struct EventData
	{
		EventID eventID;
		void* userData;
	};

	class TINY_API Event
	{
		class Listener
		{
		public:
			Listener()
				: preData(nullptr)
				, nextData{ nullptr }
				, callback()
			{

			}
			~Listener()
			{
				this->preData = nullptr;
				this->nextData = nullptr;
			}

			std::function<void(const EventData&)> callback;

			void pushBack(Listener* data)
			{
				this->nextData = data;
				data->preData = this;
			}

			void pushFront(Listener* data)
			{
				this->preData = data;
				data->nextData = this;
			}

			void removeSelf()
			{
				if (this->preData)
				{
					this->preData->nextData = this->nextData;
				}

				if (this->nextData)
				{
					this->nextData->preData = this->preData;
				}
			}

			Listener* preData;
			Listener* nextData;
		};

		class ListenerList
		{
		public:
			ListenerList() : mRoot(nullptr) {}
			~ListenerList() {}

		public:
			void push(Listener* listener)
			{
				if (mRoot == nullptr)
				{
					mRoot = listener;
				}
				else
				{
					mRoot->nextData = listener;
					listener->preData = mRoot;
					mRoot = listener;
				}
			}

			Listener* mRoot;
		};

	public:
		Event() {}
		~Event() {}

	public:
		void init(int eventCount);

	public:
		void addListener(const EventID& eventID, void* master, const std::function<void(const EventData&)>& function);
		bool removeListener(void* master);
		void dispatch(const EventData& eventData);

	private:
		std::vector<ListenerList*> mList;
		std::unordered_map<void*, std::vector<Listener*>*> mListenerWithOwnerUMap;
	};


}

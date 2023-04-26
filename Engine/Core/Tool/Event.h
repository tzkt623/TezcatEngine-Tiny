#pragma once
#include "../Head/ConfigHead.h"
#include "../Head/CppHead.h"


namespace tezcat::Tiny
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
				if (this->nextData)
				{
					this->nextData->preData = this->preData;
				}

				if (this->preData)
				{
					this->preData->nextData = TinyMove(this->nextData);
				}
				else
				{
					//如果前面没有值,说明它是第一个
					//把下一个装到头上来
					list->mRoot = TinyMove(this->nextData);
				}
			}

			std::function<void(const EventData&)> callback;
			ListenerList* list = nullptr;
			Listener* preData = nullptr;
			TinyRef<Listener> nextData;
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
					auto next = TinyMove(mRoot->nextData);
					mRoot = TinyMove(next);
				}
			}

			ListenerList(const ListenerList& other)
				: mRoot(other.mRoot)
			{

			}

		public:
			void push(TinyRef<Listener>& listener)
			{
				listener->list = this;

				if (mRoot)
				{
					mRoot->preData = listener.get();
					listener->nextData = TinyMove(mRoot);
					mRoot = TinyMove(listener);
				}
				else
				{
					mRoot = TinyMove(listener);
				}
			}

			TinyRef<Listener> mRoot;
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
		std::vector<TinyURef<ListenerList>> mListenerList;
		std::unordered_map<void*, TinyURef<std::vector<TinyRef<Listener>>>> mListenerWithOwnerUMap;
	};
}

#pragma once
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

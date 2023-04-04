#pragma once
#include "Core/Head/ConfigHead.h"
#include "Core/Head/CppHead.h"


namespace tezcat::Tiny::Utility
{
	//--------------------------------------
	//
	// Event
	//
#define BindEvent_0(x) std::bind(&x, this)
#define BindEvent_1(x) std::bind(&x, this, std::placeholders::_1)
#define BindEvent_2(x) std::bind(&x, this, std::placeholders::_1, std::placeholders::_2)
#define BindEvent_3(x) std::bind(&x, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
	template<typename... Args>
	class TINY_API Event
	{
		class Data
		{
		public:
			void* master;
			std::function<void(Args...)> callback;
		};
	public:
		Event() { }
		~Event()
		{
			for (auto d : mList)
			{
				delete d;
			}
		}

	public:
		void addListener(void* master, const std::function<void(Args...)>& function)
		{
			mList.push_back(new Data{ master, std::move(function) });
		}

		bool removeListener(void* master)
		{
			auto result = std::find(mList.begin(), mList.end(),
				[master](Data* data)
				{
					return data->master == master;
				});
			if (result != mList.end())
			{
				mList.erase(result);
				return true;
			}

			return false;
		}

		void dispatch(Args... args)
		{
			for (auto d : mList)
			{
				d->callback(args...);
			}
		}

	private:
		std::vector<Data*> mList;
	};
}

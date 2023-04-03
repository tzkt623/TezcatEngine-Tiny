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
			for (auto d : m_List)
			{
				delete d;
			}
		}

	public:
		void addListener(void* master, const std::function<void(Args...)>& function)
		{
			m_List.push_back(new Data{ master, std::move(function) });
		}

		bool removeListener(void* master)
		{
			auto result = std::find(m_List.begin(), m_List.end(),
				[master](Data* data)
				{
					return data->master == master;
				});
			if (result != m_List.end())
			{
				m_List.erase(result);
				return true;
			}

			return false;
		}

		void dispatch(Args... args)
		{
			for (auto d : m_List)
			{
				d->callback(args...);
			}
		}

	private:
		std::vector<Data*> m_List;
	};
}
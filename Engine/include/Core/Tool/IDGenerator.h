#pragma once

#include "Core/Head/CppHead.h"
#include "Core/Head/ConfigHead.h"

namespace tezcat::Tiny
{
	//-------------------------------
	//
	//  [1/27/2022 Administrator]
	//
	//
	template<class T, typename R>
	class TINY_API IDGenerator
	{
		IDGenerator() = delete;
		~IDGenerator() = delete;
	public:
		static R generate()
		{
			if (sFreePool.empty())
			{
				return mID++;
			}
			else
			{
				auto r = sFreePool.front();
				sFreePool.pop_front();
				return r;
			}
		}

		static void recycle(R&& value)
		{
			sFreePool.emplace_back(std::forward<R>(value));
		}

		static void recycle(R& value)
		{
			sFreePool.emplace_back(value);
		}

		static std::deque<R> sFreePool;
		static R mID;
	};

	template<class T, typename R>
	std::deque<R> IDGenerator<T, R>::sFreePool;

	template<class T, typename R>
	R IDGenerator<T, R>::mID;

	/*
	template<class T>
	class TINY_API IDGenerator<T, uint32_t>
	{
	public:
		IDGenerator() = delete;
		~IDGenerator() = delete;

		static uint32_t generate()
		{
			return mID++;
		}

	private:
		static uint32_t mID;
	};

	template<class T>
	unsigned int IDGenerator<T, uint32_t>::mID = 0;


	template<class T>
	class TINY_API IDGenerator<T, int>
	{
	public:
		IDGenerator() = delete;
		~IDGenerator() = delete;

		static int generate()
		{
			return mID++;
		}

	private:
		static int mID;
	};

	template<class T>
	int IDGenerator<T, int>::mID = 0;
	*/
}

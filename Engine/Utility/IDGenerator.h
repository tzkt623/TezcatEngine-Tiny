#pragma once

#include "Core/Head/CppHead.h"
#include "Core/Head/ConfigHead.h"

namespace tezcat::Tiny::Utility
{
	//-------------------------------
	//
	//  [1/27/2022 Administrator]
	//
	//
	template<class T, typename R>
	class TINY_API IDGenerator
	{
	public:
		IDGenerator() = delete;
		~IDGenerator() = delete;

		static R generate()
		{

		}
	};

	template<class T>
	class TINY_API IDGenerator<T, uint32_t>
	{
	public:
		IDGenerator() = delete;
		~IDGenerator() = delete;

		static uint32_t generate()
		{
			return m_ID++;
		}

	private:
		static uint32_t m_ID;
	};

	template<class T>
	unsigned int IDGenerator<T, uint32_t>::m_ID = 0;


	template<class T>
	class TINY_API IDGenerator<T, int>
	{
	public:
		IDGenerator() = delete;
		~IDGenerator() = delete;

		static int generate()
		{
			return m_ID++;
		}

	private:
		static int m_ID;
	};

	template<class T>
	int IDGenerator<T, int>::m_ID = 0;
}
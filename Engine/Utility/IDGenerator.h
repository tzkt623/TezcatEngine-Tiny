#pragma once

namespace tezcat::Tiny::Utility
{
	template<class T, typename R>
	class IDGenerator
	{
	public:
		IDGenerator() = delete;
		~IDGenerator() = delete;

		static R generate()
		{
			
		}
	};

	template<class T>
	class IDGenerator<T, unsigned int>
	{
	public:
		IDGenerator() = delete;
		~IDGenerator() = delete;

		static unsigned int generate()
		{
			return m_ID++;
		}

	private:
		static unsigned int m_ID;
	};

	template<class T>
	unsigned int IDGenerator<T, unsigned int>::m_ID;
}
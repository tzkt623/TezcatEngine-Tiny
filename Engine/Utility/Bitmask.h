#pragma once

namespace tezcat::Tiny::Utility
{
	template<int size>
	class Bitmask
	{
	public:
		void set() {}

	private:
		unsigned char m_Data[size];
	};
}


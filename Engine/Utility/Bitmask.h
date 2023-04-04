#pragma once

namespace tezcat::Tiny::Utility
{
	template<int size>
	class Bitmask
	{
	public:
		void set() {}

	private:
		unsigned char mData[size];
	};
}


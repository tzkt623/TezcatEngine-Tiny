#pragma once

namespace tezcat::Tiny
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


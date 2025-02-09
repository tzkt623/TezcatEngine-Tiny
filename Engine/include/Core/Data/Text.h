#pragma once

#include "../Base/TinyObject.h"

namespace tezcat::Tiny
{
	class TINY_API Text : public TinyObject
	{
	public:
		void setData(std::string&& data)
		{
			mData = std::move(data);
		}

		void setData(const std::string& data)
		{
			mData = data;
		}

		void moveData(std::string& data)
		{
			mData = std::move(data);
		}

	private:
		std::string mData;
	};
}

#pragma once

#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"

namespace tezcat::Tiny::Core
{

	/*
	 * 每一个物体同一时间只能存在一个Layer中
	 */
	class TINY_API LayerMask
	{
		LayerMask() {}
		~LayerMask() {}
	public:
		
		const uint32_t& getUID() { return mUID; }
		const std::string& getName() const
		{
			return sMaskWithName[mUID];
		}

	public:
		uint32_t mUID;

	public:
		static void init();

		static const std::string& getName(int index);
		static void setMask(int index, const std::string& maskName);

		static uint32_t getMask(const std::string& maskName);
		static uint32_t getMask(uint32_t index);

		static LayerMask* createMask(const std::string& name);

	private:
		static std::vector<std::string> sMaskWithName;
	};
}

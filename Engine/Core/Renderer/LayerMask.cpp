#include "LayerMask.h"

namespace tezcat::Tiny
{
	std::vector<std::string> LayerMask::sMaskWithName(32);

	void LayerMask::init()
	{
		sMaskWithName[0] = "default";
	}

	const std::string& LayerMask::getName(int index)
	{
		return sMaskWithName[index];
	}

	void LayerMask::setMask(int index, const std::string& maskName)
	{
		sMaskWithName[index] = maskName;
	}

	uint32_t LayerMask::getMask(const std::string& maskName)
	{
		for (int i = 0; i < sMaskWithName.size(); i++)
		{
			if (sMaskWithName[i] == maskName)
			{
				return 1 << i;
			}
		}

		return 0;
	}

	uint32_t LayerMask::getMask(uint32_t index)
	{
		return 1 << index;
	}
}


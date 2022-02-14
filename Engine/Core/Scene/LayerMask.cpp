#include "LayerMask.h"

namespace tezcat::Tiny::Core
{
	std::vector<std::string> LayerMask::m_MaskWithName(32);

	void LayerMask::init()
	{
		m_MaskWithName[0] = "default";
	}

	const std::string& LayerMask::getName(int index)
	{
		return m_MaskWithName[index];
	}

	void LayerMask::setMask(int index, const std::string& maskName)
	{
		m_MaskWithName[index] = maskName;
	}

	uint32_t LayerMask::getMask(const std::string& maskName)
	{
		for (int i = 0; i < m_MaskWithName.size(); i++)
		{
			if (m_MaskWithName[i] == maskName)
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


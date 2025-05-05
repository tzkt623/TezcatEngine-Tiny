/*
	Copyright (C) 2022 - 2025 Tezcat(特兹卡特) tzkt623@qq.com

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "Core/Shader/ShaderPackage.h"
#include "Core/Shader/Shader.h"

#include "Core/Tool/Tool.h"

#include "Core/Manager/ShaderManager.h"

namespace tezcat::Tiny
{
	ShaderPackage::ShaderPackage()
		: mUID(-1)
	{

	}


	ShaderPackage::~ShaderPackage()
	{

	}

	void ShaderPackage::setConfig(std::unordered_map<std::string, Any>& data)
	{
		mName = data["Name"].cast<std::string>();
	}

	void ShaderPackage::addShader(Shader* shader)
	{
		mShaders.push_back(shader);
	}

	void ShaderPackage::apply()
	{
		//mUID = ShaderMgr::getInstance()->giveUID();
		this->sort();
	}

	void ShaderPackage::sort()
	{
		if (mShaders.size() > 1)
		{
			std::sort(mShaders.begin(), mShaders.end(),
				[](Shader* a, Shader* b)
				{
					return a->getSortingOffset() < b->getSortingOffset();
				});
		}
	}

	void ShaderPackage::rebuild()
	{
		//ShaderMgr::getInstance()->rebuild(this);
	}

	Shader* ShaderPackage::findShader(const std::string& name)
	{
		auto it = std::find_if(mShaders.begin(), mShaders.end(), [&](Shader* a)
		{
			return a->getName() == name;
		});

		if (it != mShaders.end())
		{
			return *it;
		}

		return nullptr;
	}
}

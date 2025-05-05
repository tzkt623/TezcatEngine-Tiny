#pragma once
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
#include "Core/Head/ConfigHead.h"
#include "Core/Head/CppHead.h"
#include "StringTool.h"


namespace tezcat::Tiny
{
	//--------------------------------
	//
	// Singleton
	//
	template<typename T>
	class TINY_API SG
	{
	private:
		SG() = delete;
		SG(const SG&) = delete;
		~SG() = delete;

	public:
		template<typename... Args>
		static void init(Args&&... args)
		{
			if (mIsAttached)
			{
				throw std::logic_error(fmt::format("SG<>: This Instance is an AttachedType [{}]",
					typeid(T).name()));
			}

			mIsAttached = true;
			mInstance = new T(std::forward<Args>(args)...);
		}

		static void destroy()
		{
			if (mIsAttached)
			{
				throw std::logic_error(StringTool::stringFormat("SG<>: This Instance is an AttachedType [%s]",
					typeid(T).name()));
			}

			delete mInstance;
		}

		static void attach(T* instance)
		{
			if (mInstance != nullptr)
			{
				delete instance;
				return;
				throw std::logic_error(StringTool::stringFormat("SG<>: Instance must be null [%s]",
					typeid(T).name()));
			}

			mIsAttached = true;
			mInstance = instance;
		}

		static void detach()
		{
			if (mIsAttached)
			{
				mInstance = nullptr;
			}
			else
			{
				throw std::logic_error(StringTool::stringFormat("SG<>: This Instance is Not an AttachedType [%s]",
					typeid(T).name()));
			}
		}

		static T* getInstance()
		{
			return mInstance;
		}

	private:
		static T* mInstance;
		static bool mIsAttached;
	};

	template<typename T>
	bool SG<T>::mIsAttached = false;

	template<typename T>
	T* SG<T>::mInstance = nullptr;
}

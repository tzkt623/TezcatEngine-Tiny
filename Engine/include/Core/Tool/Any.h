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

namespace tezcat::Tiny
{
	//---------------------------------------------------------
	//
	// Any
	// 
	//
	class TINY_API Any
	{
		struct Container
		{
			virtual ~Container() {}
			virtual Container* clone() const = 0;
		};

		template<typename T>
		struct ContainerT : Container
		{
			ContainerT(T&& other) noexcept
				: mValue(std::forward<T>(other))
			{
			}

			ContainerT(const T& other)
				: mValue(other)
			{
			}

			Container* clone() const final
			{
				return new ContainerT<T>(mValue);
			}

			T mValue;

			ContainerT& operator=(const ContainerT&) = delete;
		};
	public:
		Any()
			: mTypeIndex(typeid(void))
			, mContainer(nullptr)
		{
		}

		Any(const Any& other)
			: mTypeIndex(other.mTypeIndex)
			, mContainer(other.clone())
		{
		}

		Any(Any&& other) noexcept
			: mTypeIndex(other.mTypeIndex)
			, mContainer(other.mContainer)
		{
			other.mContainer = nullptr;
		}

		template<typename T>
		Any(T& value)
			: mTypeIndex(typeid(T))
			, mContainer(new ContainerT<T>(value))
		{
		}

		template<typename T>
		Any(T&& value)
			: mTypeIndex(typeid(T))
			, mContainer(new ContainerT<T>(std::forward<T>(value)))
		{
		}

		~Any()
		{
			delete mContainer;
		}

	private:
		Container* clone() const
		{
			if (mContainer != nullptr)
			{
				return mContainer->clone();
			}

			return nullptr;
		}

	public:
		bool empty() { return mContainer == nullptr; }

		template<class U>
		bool is() const
		{
			return mTypeIndex == std::type_index(typeid(U));
		}

		template<class U>
		U& cast()
		{
			if (!this->is<U>())
			{
				throw std::bad_cast();
			}

			return dynamic_cast<ContainerT<U>*>(mContainer)->mValue;
		}

		Any& operator=(const Any& other)
		{
			if (mContainer == other.mContainer)
			{
				return *this;
			}

			mContainer = mContainer->clone();
			mTypeIndex = other.mTypeIndex;
			return *this;
		}

		Any& operator=(Any&& other) noexcept
		{
			if (mContainer == other.mContainer)
			{
				return *this;
			}

			delete mContainer;

			mContainer = other.mContainer;
			mTypeIndex = other.mTypeIndex;

			other.mContainer = nullptr;
			return *this;
		}

		template <class T>
		Any& operator=(const T& value)
		{
			delete mContainer;

			mContainer = new ContainerT<T>(value);
			mTypeIndex = std::type_index(typeid(T));

			return *this;
		}

		template <class T>
		Any& operator=(T&& value)
		{
			delete mContainer;

			mContainer = new ContainerT<T>(std::forward<T>(value));
			mTypeIndex = std::type_index(typeid(T));

			return *this;
		}

	private:
		std::type_index mTypeIndex;
		Container* mContainer;
	};
}

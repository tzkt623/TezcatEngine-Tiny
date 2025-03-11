#pragma once
/*
	Copyright (C) 2024 Tezcat(特兹卡特) tzkt623@qq.com

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
#include "Core/Head/CppHead.h"
#include "Core/Head/ConfigHead.h"

namespace tezcat::Tiny
{
	//------------------------------------------
	// 
	// 请使用IDString的特化版unordered_xxx
	// 
	// Please use the special unordered_xxx for IDString
	//
	// IDString::UMap IDString::USet
	//
	template<typename Belong>
	class TINY_API IDString
	{
	public:
		struct CompareHash
		{
			std::size_t operator() (const IDString& str) const
			{
				return str.mID;
			}
		};

		struct Comparer
		{
			bool operator() (const IDString& a, const IDString& b) const
			{
				return a.mID == b.mID;
			}
		};

	public:
		template<typename Value>
		using UMap = std::unordered_map<IDString, Value, CompareHash, Comparer>;
		using USet = std::unordered_set<IDString, CompareHash, Comparer>;

	public:
		IDString()
			: mID(0)
		{

		}

		IDString(const char* string)
			: mID(0)
		{
			this->init(string);
		}

		// 		IDString(std::string&& string)
		// 			: mID(0)
		// 		{
		// 			this->init(std::forward<std::string>(string));
		// 		}

		IDString(const std::string& string)
			: mID(0)
		{
			this->init(string);
		}

		IDString(const IDString& other)
			: mID(other.mID)
		{

		}

		IDString(IDString&& other) noexcept
			: mID(other.mID)
		{
			other.mID = 0;
		}

		~IDString()
		{

		}

		IDString& operator = (const char* data)
		{
			this->init(data);
			return *this;
		}

		IDString& operator = (const std::string& data)
		{
			this->init(data);
			return *this;
		}

		// 		IDString& operator = (std::string&& data)
		// 		{
		// 			this->init(std::forward<std::string>(data));
		// 			return *this;
		// 		}

		IDString& operator = (IDString&& other) noexcept
		{
			mID = other.mID;
			other.mID = 0;
			return *this;
		}

		IDString& operator = (const IDString& other)
		{
			mID = other.mID;
			return *this;
		}

		bool operator == (const IDString& other) const
		{
			return mID == other.mID;
		}

		bool operator != (const IDString& other) const
		{
			return mID != other.mID;
		}

		bool operator == (const std::string& other) const
		{
			auto it = sStringMap.find(other);
			if (it == sStringMap.end())
			{
				return false;
			}

			return mID == it->second;
		}

		bool operator != (const std::string& other) const
		{
			auto it = sStringMap.find(other);
			if (it == sStringMap.end())
			{
				return true;
			}

			return mID != it->second;
		}

		//operator const char* ()
		//{
		//	return sStringArray[mID].data();
		//}
		//
		operator uint32_t()
		{
			return mID;
		}

		//operator std::string_view()
		//{
		//	return sStringArray[mID];
		//}

		const uint32_t toUID() const
		{
			return mID;
		}

		const char* toData() const
		{
			return sStringArray[mID]->c_str();
		}

		const std::string& toString() const
		{
			return *sStringArray[mID];
		}

		const std::string_view toView() const
		{
			return *sStringArray[mID];
		}

	private:
		void init(const std::string& data)
		{
			auto it = sStringMap.find(data);
			if (it == sStringMap.end())
			{
				mID = static_cast<uint32_t>(sStringArray.size());
				sStringArray.push_back(new std::string(data));
				sStringMap.emplace(*sStringArray[mID], mID);
			}
			else
			{
				mID = it->second;
			}

			//auto pair = sStringMap.try_emplace(data, 0);
			//if (pair.second)
			//{
			//	mID = static_cast<uint32_t>(sStringArray.size());
			//	pair.first->second = mID;
			//	sStringArray.emplace_back(pair.first->first);
			//}
			//else
			//{
			//	mID = pair.first->second;
			//}
		}

		void init(const char* data)
		{
			auto it = sStringMap.find(data);
			if (it == sStringMap.end())
			{
				mID = static_cast<uint32_t>(sStringArray.size());
				sStringArray.push_back(new std::string(data));
				sStringMap.emplace(*sStringArray[mID], mID);
			}
			else
			{
				mID = it->second;
			}

			//auto pair = sStringMap.try_emplace(data, 0);
			//if (pair.second)
			//{
			//	mID = static_cast<uint32_t>(sStringArray.size());
			//	pair.first->second = mID;
			//	sStringArray.emplace_back(pair.first->first);
			//}
			//else
			//{
			//	mID = pair.first->second;
			//}
		}

	private:
		uint32_t mID;

	public:
		static auto allStringCount() { return sStringArray.size(); }

		static bool tryAddString(const std::string& data)
		{
			auto it = sStringMap.find(data);
			if (it == sStringMap.end())
			{
				mID = static_cast<uint32_t>(sStringArray.size());
				sStringArray.push_back(new std::string(data));
				sStringMap.emplace(*sStringArray[mID], mID);
				return true;
			}
			else
			{
				mID = it->second;
				return false;
			}
		}

		static IDString create(const char* data)
		{
			return IDString(data);
		}

		static std::string_view getStringStatic(const uint32_t& id)
		{
			return *sStringArray[id];
		}

		static const uint32_t getUIDStatic(const std::string& name)
		{
			TINY_THROW_RUNTIME(!sStringMap.contains(name), name);
			return sStringMap[name];
		}

		static void deleteAllString()
		{
			for (auto ptr : sStringArray)
			{
				delete ptr;
			}
			sStringArray.clear();
			sStringMap.clear();
		}

	private:
		static std::vector<std::string*> sStringArray;
		static std::unordered_map<std::string_view, uint32_t> sStringMap;
	};

	template<typename Belong>
	std::vector<std::string*> IDString<Belong>::sStringArray{ new std::string("TINY_EMPTY_IDSTRING") };

	template<typename Belong>
	std::unordered_map<std::string_view, uint32_t> IDString<Belong>::sStringMap{ {*sStringArray[0], 0} };

	template<typename Belong>
	std::ostream& operator << (std::ostream& out, IDString<Belong>& idString)
	{
		return out << idString.toString();
	}

	// 	template<typename Belong>
	// 	bool operator == (const IDString<Belong>& a, const IDString<Belong>& b)
	// 	{
	// 		return a.getUID() == b.getUID();
	// 	}
	// 
	// 	template<typename Belong>
	// 	bool operator != (const IDString<Belong>& a, const IDString<Belong>& b)
	// 	{
	// 		return a.getUID() != b.getUID();
	// 	}
}

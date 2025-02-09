#pragma once

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
			: mID(-1)
		{

		}

		IDString(const char* string)
			: mID(-1)
		{
			this->init(string);
		}

// 		IDString(std::string&& string)
// 			: mID(-1)
// 		{
// 			this->init(std::forward<std::string>(string));
// 		}

		IDString(const std::string& string)
			: mID(-1)
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
			other.mID = -1;
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
			other.mID = -1;
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

		operator const char* ()
		{
			return sStringArray[mID].data();
		}

		operator int()
		{
			return mID;
		}

		const int getUID() const
		{
			return mID;
		}

		const char* getStringData() const
		{
			return sStringArray[mID].data();
		}

		const std::string_view& getString() const
		{
			return sStringArray[mID];
		}

	private:
		void init(const std::string& data)
		{
			auto pair = sStringMap.try_emplace(data, mID);
			if (pair.second)
			{
				mID = static_cast<int>(sStringArray.size());
				pair.first->second = mID;
				sStringArray.emplace_back(pair.first->first);
			}
			else
			{
				mID = pair.first->second;
			}
		}

// 		void init(std::string&& data)
// 		{
// 			auto pair = sStringMap.try_emplace(std::move(data), mID);
// 			if (pair.second)
// 			{
// 				mID = static_cast<int>(sStringArray.size());
// 				pair.first->second = mID;
// 				sStringArray.emplace_back(pair.first->first);
// 			}
// 			else
// 			{
// 				mID = pair.first->second;
// 			}
// 		}

		void init(const char* data)
		{
			//			std::string name(data);
			auto pair = sStringMap.try_emplace(data, mID);
			if (pair.second)
			{
				mID = static_cast<int>(sStringArray.size());
				pair.first->second = mID;
				sStringArray.emplace_back(pair.first->first);
			}
			else
			{
				mID = pair.first->second;
			}
		}

	private:
		int mID;

	public:
		static auto allStringCount() { return sStringArray.size(); }

		static bool tryAddString(const std::string& data)
		{
			auto pair = sStringMap.try_emplace(data, -1);
			if (pair.second)
			{
				pair.first->second = static_cast<int>(sStringArray.size());
				sStringArray.emplace_back(pair.first->first);
				return true;
			}

			return false;
		}

		static IDString create(const char* data)
		{
			return IDString(data);
		}

		static const std::string_view& getStringStatic(const int& id)
		{
			return sStringArray[id];
		}

		static const int getUIDStatic(const std::string& name)
		{
			TINY_THROW_RUNTIME(!sStringMap.contains(name), name);
			return sStringMap[name];
		}

	private:
		static std::unordered_map<std::string, int> sStringMap;
		static std::vector<std::string_view> sStringArray;
	};

	template<typename Belong>
	std::unordered_map<std::string, int> IDString<Belong>::sStringMap;

	template<typename Belong>
	std::vector<std::string_view> IDString<Belong>::sStringArray;

	template<typename Belong>
	std::ostream& operator << (std::ostream& out, IDString<Belong>& idString)
	{
		return out << idString.getString();
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

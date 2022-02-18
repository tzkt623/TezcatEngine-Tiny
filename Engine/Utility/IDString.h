#pragma once

#include "Core/Head/CppHead.h"
#include "Core/Head/ConfigHead.h"

namespace tezcat::Tiny::Utility
{
	//------------------------------------------
	// 
	// 请使用IDString特制的unordered容器
	// 
	// please use the special unordered_xx for IDString
	//
	// IDString::UMap IDString::USet
	template<typename Belong>
	class TINY_API IDString
	{
	public:
		struct CompareHash
		{
			std::size_t operator() (const IDString& str) const
			{
				return str.m_ID;
			}
		};

		struct Comparer
		{
			bool operator() (const IDString& a, const IDString& b) const
			{
				return a.m_ID == b.m_ID;
			}
		};

	public:
		template<typename Value>
		using UMap = std::unordered_map<IDString, Value, CompareHash, Comparer>;
		using USet = std::unordered_set<IDString, CompareHash, Comparer>;

	public:
		IDString()
			: m_ID(-1)
		{

		}

		IDString(const char* string)
			: m_ID(-1)
		{
			this->init(string);
		}

		IDString(std::string&& string)
			: m_ID(-1)
		{
			this->init(std::forward<std::string>(string));
		}

		IDString(const std::string& string)
			: m_ID(-1)
		{
			this->init(string);
		}

		IDString(const IDString& other)
			: m_ID(other.m_ID)
		{

		}

		IDString(IDString&& other) noexcept
			: m_ID(other.m_ID)
		{
			other.m_ID = -1;
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

		IDString& operator = (std::string&& data)
		{
			this->init(std::forward<std::string>(data));
			return *this;
		}

		IDString& operator = (IDString&& other) noexcept
		{
			m_ID = other.m_ID;
			other.m_ID = -1;
			return *this;
		}

		IDString& operator = (const IDString& other)
		{
			m_ID = other.m_ID;
			return *this;
		}

		bool operator == (const IDString& other) const
		{
			return m_ID == other.m_ID;
		}

		bool operator != (const IDString& other) const
		{
			return m_ID != other.m_ID;
		}

		operator int()
		{
			return m_ID;
		}

		operator const char* ()
		{
			return s_StringArray[m_ID].data();
		}

		inline const int getUID() const
		{
			return m_ID;
		}

		const char* getStringData() const
		{
			return s_StringArray[m_ID].data();
		}

		const std::string_view& getString() const
		{
			return s_StringArray[m_ID];
		}

	private:
		void init(const std::string& data)
		{
			auto pair = s_StringMap.try_emplace(data, m_ID);
			if (pair.second)
			{
				m_ID = static_cast<int>(s_StringArray.size());
				pair.first->second = m_ID;
				s_StringArray.emplace_back(pair.first->first);
			}
			else
			{
				m_ID = pair.first->second;
			}
		}

		void init(std::string&& data)
		{
			auto pair = s_StringMap.try_emplace(std::move(data), m_ID);
			if (pair.second)
			{
				m_ID = static_cast<int>(s_StringArray.size());
				pair.first->second = m_ID;
				s_StringArray.emplace_back(pair.first->first);
			}
			else
			{
				m_ID = pair.first->second;
			}
		}

		void init(const char* data)
		{
			//			std::string name(data);
			auto pair = s_StringMap.try_emplace(data, m_ID);
			if (pair.second)
			{
				m_ID = static_cast<int>(s_StringArray.size());
				pair.first->second = m_ID;
				s_StringArray.emplace_back(pair.first->first);
			}
			else
			{
				m_ID = pair.first->second;
			}
		}

	private:
		int m_ID;

	public:
		static auto allStringCount() { return s_StringArray.size(); }

		static bool tryAddString(const std::string& data)
		{
			auto pair = s_StringMap.try_emplace(data, -1);
			if (pair.second)
			{
				pair.first->second = static_cast<int>(s_StringArray.size());
				s_StringArray.emplace_back(pair.first->first);
				return true;
			}

			return false;
		}

		static IDString create(const char* data)
		{
			return IDString(data);
		}

	private:
		static std::unordered_map<std::string, int> s_StringMap;
		static std::vector<std::string_view> s_StringArray;
	};

	template<typename Belong>
	std::unordered_map<std::string, int> IDString<Belong>::s_StringMap;

	template<typename Belong>
	std::vector<std::string_view> IDString<Belong>::s_StringArray;

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
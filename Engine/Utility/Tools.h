#pragma once

#include <string>
#include <vector>
#include <cstdio>
#include <list>
#include <memory>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <initializer_list>
#include <regex>


namespace tezcat::Tiny::Utility
{
	class Tools
	{
	public:
		static std::string fromU8(const std::u8string& string);

		template<typename... Args>
		static std::string stringFormat(const std::string& format, Args... args)
		{
			auto str = format.c_str();
			auto size = 1 + std::snprintf(nullptr, 0, str, args...);
			if (s_StringBuffer.capacity() < size)
			{
				s_StringBuffer.resize(size);
			}
			std::snprintf(s_StringBuffer.data(), size, str, args...);
			return std::string(s_StringBuffer.data(), size);
		}

		template<typename... Args>
		static int stringFormat(std::string& container, const std::string& format, Args... args)
		{
			auto str = format.c_str();

			auto size = 1 + std::snprintf(nullptr, 0, str, args...);
			if (s_StringBuffer.capacity() < size)
			{
				s_StringBuffer.resize(size);
			}
			std::snprintf(s_StringBuffer.data(), size, str, args...);

			container.append(s_StringBuffer.data(), size);
			return size;
		}

	private:
		static std::vector<char> s_StringBuffer;
	};


	//-------------------------------
	//
	//  [1/27/2022 Administrator]
	//
	//
	template<class T, typename R>
	class IDGenerator
	{
	public:
		IDGenerator() = delete;
		~IDGenerator() = delete;

		static R generate()
		{

		}
	};

	template<class T>
	class IDGenerator<T, unsigned int>
	{
	public:
		IDGenerator() = delete;
		~IDGenerator() = delete;

		static unsigned int generate()
		{
			return m_ID++;
		}

	private:
		static unsigned int m_ID;
	};

	template<class T>
	unsigned int IDGenerator<T, unsigned int>::m_ID = 0;

	//---------------------------------------------------------------------------------
	//
	// 
	//   [1/27/2022 Administrator]
	//
	//
#define CREATE_DELEGATE_INSTANCE(class_name)\
private:\
	static class_name* m_Instance##class_name;\
public:\
	static class_name* getInstance() { return m_Instance##class_name; }

#define IMPLEMENT_DELEGATE_INSTANCE(class_name) class_name* class_name::m_Instance##class_name = nullptr

#define REGISTER_DELEGATE_INSTANCE(class_name)\
if(m_Instance##class_name != nullptr)\
{\
	delete m_Instance##class_name;\
}\
m_Instance##class_name = this

#define STATIC_IMPLEMENT(class_name, valut_type, value_name, value) static valut_type class_name::value_name = value
}


namespace tezcat::Tiny {}
namespace tezcat::Tiny::Core {}
namespace tezcat::Tiny::Module {}
namespace tezcat::Tiny::Utility {}

#define USING_ALL_NAMESPACE()\
using namespace tezcat::Tiny;\
using namespace tezcat::Tiny::Core;\
using namespace tezcat::Tiny::Module;\
using namespace tezcat::Tiny::Utility;


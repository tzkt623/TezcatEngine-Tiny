﻿#pragma once

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

#include "TinyGC.h"
#include "../Tool/IDString.h"

namespace tezcat::Tiny
{
	struct TINY_API TinyRTTI
	{
		const TinyRTTI* __parent;
		const std::string __className;
		const std::type_index __typeIndex;
		const uint32_t __classID;
	};

	template<class Self, class Parent>
	struct TINY_API TinyRTTIT : public TinyRTTI
	{
		Parent* convertToParent(Self* self)
		{
			return (Parent*)self;
		}
	};

	class TinyObject;
	class TinyRefObject;
	/*
	* TinyRefObject
	* @brief 引用对象,靠引用计数来管理自身生命周期
	* @brief addRef增加引用
	* @brief subRef减少引用
	*
	*/

	using TinyIDString = IDString<TinyRefObject>;

	class TINY_API TinyRefObject
	{
		friend class TinyBaseWeakRef;
		friend class TinyObject;
		friend class TinyBaseRef;

	public:
		static uint32_t giveID() { return __ClassID++; }
		TinyGCInfo* mGCInfo;

	private:
		static const std::string Empty;
		static const TinyRTTI __TINY__RTTI__453;
		static uint32_t __ClassID;

	public:
		TinyRefObject();
		virtual ~TinyRefObject();

		int getRefCount() const { return mGCInfo->strongRef; }
		//save current object
		void saveObject() { ++mGCInfo->strongRef; }

		//use this to delete object
		void deleteObject();

		virtual std::string getMemoryInfo() { return this->getClassName(); }

		TinyIDString getEngineName() { return mEngineName; }
		void setEngineName(const std::string& name) { mEngineName = name; }


	public:
		TinyGCInfoID getTinyID() const { return mGCInfo->index; }
		bool isValid() const { return mGCInfo->strongRef > 0; }

		//RTTI
	public:
		virtual const std::string& getClassName() const { return __TINY__RTTI__453.__className; }
		virtual const std::string& getParentClassName() const = 0;
		virtual const std::type_index& getClassType() const { return __TINY__RTTI__453.__typeIndex; }
		virtual const uint32_t getClassID() const { return __TINY__RTTI__453.__classID; }

		virtual const TinyRTTI& RTTI() const { return __TINY__RTTI__453; }
		static const TinyRTTI* RTTIStatic() { return &__TINY__RTTI__453; }

		template<class OBJECT>
		bool typeEqual()
		{
			return OBJECT::RTTIStatic()->__typeIndex == this->RTTI().__typeIndex;
		}

		bool typeEqual(TinyRefObject* object)
		{
			return this->RTTI().__typeIndex == object->RTTI().__typeIndex;
		}

		template<class OBJECT>
		bool typeIDEqual()
		{
			return OBJECT::RTTIStatic()->__classID == this->RTTI().__classID;
		}

		bool typeIDEqual(TinyRefObject* object)
		{
			return this->RTTI().__classID == object->RTTI().__classID;
		}



	protected:
		void autoGC();
		virtual void init() {}

		virtual void onClose()
		{

		}

	protected:
		TinyIDString mEngineName;
	};

	//-----------------------------------------------------
	//
	//	共享引用
	//
	class TINY_API TinyBaseRef
	{
	public:
		template<class T>
		using Type = std::enable_if_t<std::is_convertible_v<T*, TinyRefObject*>, T>;

		template<typename To>
		using ToType = std::enable_if_t<std::is_convertible_v<To*, TinyRefObject*>, To>;

	public:
		TinyBaseRef() = default;
		virtual ~TinyBaseRef() = default;

	public:
		virtual TinyRefObject* getTinyRefObject() = 0;
		TinyGCInfo* getGCInfo() { return mGCInfo; }


	protected:
		TinyGCInfo* mGCInfo = nullptr;
	};

	class TINY_API TinyBaseLifeRef : public TinyBaseRef
	{
		virtual ~TinyBaseLifeRef()
		{

		}
	};

	template<class T>
	class TINY_API TinyUniqueRef : public TinyBaseRef
	{
	public:
		TinyUniqueRef()
		{

		}

		TinyUniqueRef(Type<T>* obj)
			: mObject(obj)
		{
			if (obj == nullptr)
			{
				return;
			}

			mGCInfo = mObject->mGCInfo;
			if (mGCInfo->strongRef != 1 || mGCInfo->unique)
			{
				abort();
			}

			mGCInfo->unique = true;
			mGCInfo->strongRef = 2;
		}

		void reset(Type<T>* obj)
		{
			if (obj == nullptr)
			{
				mObject->deleteObject();
			}
			else
			{

			}

			if (mGCInfo->strongRef != 1 || mGCInfo->unique)
			{
				abort();
			}

			mGCInfo->unique = true;
			mGCInfo->strongRef = 2;
			mObject = obj;
		}

		TinyUniqueRef(const TinyUniqueRef& other) = delete;

		TinyUniqueRef(TinyUniqueRef&& other) noexcept
		{
			if (mGCInfo->strongRef != 1 || !mGCInfo->unique)
			{
				abort();
			}

			mObject = other.mObject;
			other.mGCInfo = nullptr;
			other.mObject = nullptr;
		}

		virtual ~TinyUniqueRef()
		{
			if (mObject && mGCInfo)
			{
				mObject->deleteObject();
			}

			mObject = nullptr;
			mGCInfo = nullptr;
		}

	public:
		virtual TinyRefObject* getTinyRefObject() { return mObject; }

		TinyUniqueRef& operator=(const TinyUniqueRef& other) = delete;

		Type<T>* operator->()
		{
			return mObject;
		}

		Type<T>* operator->() const
		{
			return mObject;
		}

		Type<T>* get() const
		{
			return mObject;
		}

	private:
		Type<T>* mObject;
	};

	template<class T>
	class TINY_API TinySharedRef : public TinyBaseRef
	{
	public:
		TinySharedRef()
			: mObject(nullptr)
		{

		}

		TinySharedRef(Type<T>* obj)
		{
			this->bind(obj);
		}

		TinySharedRef(const TinySharedRef& other) : TinyBaseRef(other.mObject)
		{
			this->bind(other.mObject);
		}

		virtual ~TinySharedRef()
		{
			mObject->deleteObject();
			mObject = nullptr;
			mGCInfo = nullptr;
		}

		virtual TinyRefObject* getTinyRefObject() override { return mObject; }

		void bind(Type<T>* obj)
		{
			if (obj->mGCInfo->unique)
			{
				abort();
			}

			if (mObject)
			{
				mObject->deleteObject();
			}

			mObject = obj;
			mObject->saveObject();
			mGCInfo = mObject->mGCInfo;
		}

	public:
		TinySharedRef& operator=(const TinySharedRef& other)
		{
			this->bind(other.mObject);
			return *this;
		}

		Type<T>* operator->()
		{
			return mObject;
		}

		Type<T>* operator->() const
		{
			return mObject;
		}

		Type<T>* get() const
		{
			return mObject;
		}

	private:
		Type<T>* mObject;
	};

	//---------------------------------------------------
	//
	// WeakRef
	//
	template<typename T>
	class TINY_API TinyWeakRef : public TinyBaseRef
	{
		Type<T>* mObject;

	public:
		TinyWeakRef()
			: mObject(nullptr)
		{
			mGCInfo = TinyGC::getDefaultGCInfo();
		}

		TinyWeakRef(std::nullptr_t)
			: mObject(nullptr)
		{
			mGCInfo = TinyGC::getDefaultGCInfo();
		}

		TinyWeakRef(Type<T>* obj)
			: mObject(obj)
		{
			if (obj)
			{
				if (obj->mGCInfo->unique)
				{
					abort();
				}

				mGCInfo = mObject->mGCInfo;
				mGCInfo->weakRef++;
			}
			else
			{
				mGCInfo = TinyGC::getDefaultGCInfo();
			}
		}

		TinyWeakRef(const TinyWeakRef& other)
		{
			mGCInfo = TinyGC::getDefaultGCInfo();
			this->bind(other);
		}

		TinyWeakRef(TinyWeakRef&& other) noexcept
		{
			mGCInfo = other.mGCInfo;
			mObject = other.mObject;

			other.mGCInfo = TinyGC::getDefaultGCInfo();
			other.mObject = nullptr;
		}

		virtual ~TinyWeakRef()
		{
			this->release();

			mGCInfo = nullptr;
			mObject = nullptr;
		}

		virtual TinyRefObject* getTinyRefObject() override { return mObject; }

		void reset(Type<T>* other)
		{
			if (mObject == other)
			{
				return;
			}

			this->release();
			if (other == nullptr)
			{
				return;
			}

			mGCInfo = other->mGCInfo;
			mGCInfo->weakRef++;
			mObject = other;
		}

		Type<T>* get() const { return mObject; }

		Type<T>* lock()
		{
			if (mGCInfo->strongRef < 1)
			{
				return nullptr;
			}

			return mObject;
		}

		Type<T>* lock() const
		{
			if (mGCInfo->strongRef < 1)
			{
				return nullptr;
			}

			return mObject;
		}

		template<class To>
		TinyWeakRef<ToType<To>> staticCast()
		{
			return TinyWeakRef(static_cast<To*>(mObject));
		}

		template<class To>
		TinyWeakRef<ToType<To>> dynamicCast()
		{
			return TinyWeakRef(dynamic_cast<To*>(mObject));
		}

	public:
		TinyWeakRef& operator=(const TinyWeakRef& other)
		{
			this->bind(other);
			return *this;
		}

		Type<T>* operator->()
		{
			return mObject;
		}

		Type<T>* operator->() const
		{
			return mObject;
		}

	private:
		void release()
		{
			if (mGCInfo == TinyGC::getDefaultGCInfo())
			{
				return;
			}

			if ((--mGCInfo->weakRef) < 1)
			{
				if (mGCInfo->strongRef < 1)
				{
					TinyGC::recycle(mGCInfo);
				}
			}

			mGCInfo = TinyGC::getDefaultGCInfo();
		}

		void bind(const TinyWeakRef& other)
		{
			this->release();
			mGCInfo = other.mGCInfo;
			mObject = other.mObject;
			mGCInfo->weakRef++;
		}
	};

	//using Base = std::enable_if_t<std::is_convertible_v<currentClass, parentClass>, parentClass>;

// 	template <class Current, class Parent>
// 	concept BaseCheck = requires
// 	{
// 		std::is_convertible_v<Current*, Parent*>
// 	};
// 
// 	template<class Current, class Parent, bool test>
// 	struct BaseChecker
// 	{
// 
// 	};

	template<class Current, class Parent>
	concept BPRule = std::is_convertible_v<Current*, Parent*>;

	template<class Current, class Parent>
	requires BPRule<Current, Parent>
	struct GetParentClassType
	{
		using type = Parent;
	};

#define TINY_TEMPLATE_NAME(x) x

#define TINY_ABSTRACT_OBJECT_H(currentClass, parentClass)\
public:\
		/*using Base = GetParentClassType<currentClass, parentClass>::type;*/\
		using Base = parentClass;\
public:\
		constexpr static const std::string& staticGetClassName() { return __TINY__RTTI__453.__className; }\
		constexpr static const std::string& staticGetParentClassName() { return __TINY__RTTI__453.__parent->__className; }\
		virtual const std::string& getClassName() const override { return __TINY__RTTI__453.__className; }\
		virtual const std::string& getParentClassName() const override { return __TINY__RTTI__453.__parent->__className; }\
		virtual const std::type_index& getClassType() const override { return __TINY__RTTI__453.__typeIndex; }\
		virtual const uint32_t getClassID() const override { return __TINY__RTTI__453.__classID; }\
		static const TinyRTTI* RTTIStatic() { return &__TINY__RTTI__453; }\
		virtual const TinyRTTI& RTTI() const override { return __TINY__RTTI__453; }\
private:\
		static const TinyRTTI __TINY__RTTI__453;

#define TINY_OBJECT_H_VISIT(visit, currentClass, parentClass)\
visit:\
	template<typename... Args>\
	static currentClass* create(Args&&... args)\
	{\
		currentClass* obj = new currentClass(TINY_FWD(args)...);\
		obj->autoGC();\
		obj->init();\
		return obj;\
	}\
	TINY_ABSTRACT_OBJECT_H(currentClass, parentClass)


#define TINY_OBJECT_H(currentClass, parentClass) TINY_OBJECT_H_VISIT(public, currentClass, parentClass)

#define TINY_OBJECT_H_TEMPLATE(currentClass, parentClass, ...) TINY_OBJECT_H(currentClass, parentClass<__VA_ARGS__>)


#define TINY_OBJECT_CPP(currentClass, parentClass)\
	const TinyRTTI currentClass::__TINY__RTTI__453 =\
	{\
		parentClass::RTTIStatic(),\
		#currentClass,\
		typeid(currentClass),\
		TinyRefObject::giveID()\
	}

#define	TINY_OBJECT_CPP_TEMPLATE(currentClass, parentClass, ...) TINY_OBJECT_CPP(currentClass, parentClass<__VA_ARGS__>)

#define TINY_OBJECT_MEMORY_INFO_WTIH_NAME(engineName) std::format("{}", engineName.toString())
#define TINY_OBJECT_MEMORY_INFO() TINY_OBJECT_MEMORY_INFO_WTIH_NAME(mEngineName)
}

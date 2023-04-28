#pragma once
#include "../Head/CppHead.h"
#include "TinyGC.h"

namespace tezcat::Tiny
{
	class TINY_API TinyRefObject
	{
		friend class TinyBaseWeakRef;
	public:
		TinyRefObject();
		virtual ~TinyRefObject();

		void manageThis();

		int getRefCount() { return mGCInfo->strongRef; }

		void addRef() { ++mGCInfo->strongRef; }
		void subRef();

		virtual const std::string& getClassName() const { return Empty; }
		const TinyGCInfoID& getTinyID() const { return mGCInfo->index; }

	private:
		TinyGCInfo* mGCInfo;

	private:
		static const std::string Empty;
	};

	//-------------------------------------
	//
	// 弱引用
	// 用于在对象主动删除自己之后
	// 还留在别的容器中时进行生命存活的判断
	//
	class TINY_API TinyBaseWeakRef
	{
	public:
		constexpr TinyBaseWeakRef() noexcept
			: mGCInfo(TinyGC::DefaultGCInfo)
		{
			++mGCInfo->weakRef;
		}

		constexpr TinyBaseWeakRef(std::nullptr_t) noexcept
			: mGCInfo(TinyGC::DefaultGCInfo) {}

		TinyBaseWeakRef(TinyRefObject* obj)
			: mGCInfo(obj ? obj->mGCInfo : TinyGC::DefaultGCInfo)
		{
			++mGCInfo->weakRef;
		}

		virtual ~TinyBaseWeakRef();

		TinyRefObject* getTinyRefObject() { return mGCInfo->pointer; }


		void reset(TinyRefObject* other)
		{
			this->release();
			mGCInfo = other->mGCInfo;
			++(mGCInfo->weakRef);
		}

	protected:
		void release();

	protected:
		TinyGCInfo* mGCInfo = nullptr;
	};

	template<typename T>
	class TINY_API TinyWeakRef : public TinyBaseWeakRef
	{
		using Type = std::enable_if_t<
			std::is_base_of_v<TinyRefObject, T>
			&& std::is_class_v<T>
			, T>;

		template<typename To>
		using ToType = std::enable_if_t<
			std::is_base_of_v<TinyRefObject, To>
			&& std::is_class_v<To>
			, To>;

	public:
		constexpr TinyWeakRef()
			: TinyBaseWeakRef()
		{}
		constexpr TinyWeakRef(std::nullptr_t) noexcept
			: TinyBaseWeakRef()
		{}


		TinyWeakRef(Type* obj)
			: TinyBaseWeakRef(obj)
		{

		}

		TinyWeakRef(const TinyWeakRef& other)
		{
			this->setOther(other);
		}

		TinyWeakRef(TinyWeakRef&& other)
		{
			mGCInfo = other.mGCInfo;
			other.mGCInfo = nullptr;
		}

		virtual ~TinyWeakRef() {}

		Type* get() { return  static_cast<Type*>(mGCInfo->pointer); }

		Type* lock()
		{
			if (mGCInfo->strongRef < 1)
			{
				return nullptr;
			}

			return static_cast<Type*>(mGCInfo->pointer);
		}

		template<class To>
		TinyWeakRef<ToType<To>> staticCast()
		{
			return std::move(new TinyWeakRef(static_cast<Type*>(mGCInfo->pointer)));
		}

		template<class To>
		TinyWeakRef<ToType<To>> dynamicCast()
		{
			return std::move(new TinyWeakRef(dynamic_cast<Type*>(mGCInfo->pointer)));
		}

	public:
		TinyWeakRef& operator=(const TinyWeakRef& other)
		{
			this->setOther(other);
			return *this;
		}

		Type* operator->()
		{
			return static_cast<Type*>(mGCInfo->pointer);
		}

	private:
		void setOther(const TinyWeakRef& other)
		{
			this->release();
			mGCInfo = other.mGCInfo;
			++(mGCInfo->weakRef);
		}
	};




#define TINY_Factory(className)\
public:\
	template<typename... Args>\
	static className* create(Args&&... args)\
	{\
		auto obj = new className(std::forward<Args>(args)...);\
		obj->manageThis();\
		return obj;\
	}\
private:

#define TINY_RTTI_H(className)\
private:\
	static const std::string __rttiClassName;\
public:\
	constexpr static const std::string& getClassNameStatic() { return __rttiClassName; }\
	const std::string& getClassName() const override { return __rttiClassName; }


#define TINY_RTTI_CPP(className)\
const std::string className::__rttiClassName = #className;
}

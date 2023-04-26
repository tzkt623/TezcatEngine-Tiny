#pragma once
#include "../Head/CppHead.h"

namespace tezcat::Tiny
{
	class TinyRefObject;
	struct TinyGCInfo
	{
		uint32_t index = 0;
		int strongRef = 0;
		int weakRef = 0;
		TinyRefObject* pointer = nullptr;

		TinyGCInfo() {}
		TinyGCInfo(int strongRef, int weakRef, TinyRefObject* pointer)
			: strongRef(strongRef)
			, weakRef(weakRef)
			, pointer(pointer)
		{

		}

		TinyGCInfo(uint32_t index)
			: index(index)
		{

		}

		~TinyGCInfo()
		{
			this->pointer = nullptr;
		}

		TinyGCInfo(TinyGCInfo&& other) noexcept
		{
			this->pointer = other.pointer;
			this->strongRef = other.strongRef;
			this->weakRef = other.weakRef;

			other.strongRef = -1;
			other.weakRef = -1;
			other.pointer = nullptr;
		}

		void collect();

		static TinyGCInfo* Default;
	};

	class TINY_API TinyRefObject
	{
		friend class TinyBaseWeakRef;
	public:
		TinyRefObject();
		virtual ~TinyRefObject();

		void manageThis();

		int getRefCount() { return mGCInfo->strongRef; }

		void addRef()
		{
			++mGCInfo->strongRef;
		}

		void subRef();

		virtual const std::string& getClassName() const { return Empty; }

	private:
		TinyGCInfo* mGCInfo;
		uint32_t mUID;

	private:
		static const std::string Empty;
		static uint32_t sIDGiver;
		static std::deque<uint32_t> sFreeUIDPool;

	public:
		static uint32_t totalID() { return sIDGiver; }
		static uint32_t freeID() { return sFreeUIDPool.size(); }
		static uint32_t usedID() { return sIDGiver - sFreeUIDPool.size(); }
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
			: mGCInfo(TinyGCInfo::Default)
		{

		}

		constexpr TinyBaseWeakRef(std::nullptr_t) noexcept
			: mGCInfo(TinyGCInfo::Default) {}

		TinyBaseWeakRef(TinyRefObject* obj)
			: mGCInfo(obj ? obj->mGCInfo : TinyGCInfo::Default)
		{
			++(mGCInfo->weakRef);
		}

		virtual ~TinyBaseWeakRef();

		TinyRefObject* getTinyRefObject() { return mGCInfo->pointer; }

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
		using to_type = std::enable_if_t<
			std::is_base_of_v<TinyRefObject, To>
			&& std::is_class_v<To>
			, To>;

	public:
		constexpr TinyWeakRef() noexcept = default;
		constexpr TinyWeakRef(std::nullptr_t) noexcept {}

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
		TinyWeakRef<to_type<To>> staticCast()
		{
			return std::move(new TinyWeakRef(static_cast<Type*>(mGCInfo->pointer)));
		}

		template<class To>
		TinyWeakRef<to_type<To>> dynamicCast()
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
			if (mGCInfo == TinyGCInfo::Default)
			{
				mGCInfo = other.mGCInfo;
				++(mGCInfo->weakRef);
			}
			else
			{
				this->release();
				mGCInfo = other.mGCInfo;
				++(mGCInfo->weakRef);
			}
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

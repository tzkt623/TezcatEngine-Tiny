#pragma once

#include "../Head/CppHead.h"

namespace tezcat::Tiny
{
	class TinyRefObject;
	struct TinyGCInfo;
	class TINY_API TinyGC
	{
		static TinyGC sInstance;
	public:
		static TinyGC& getInstance() { return sInstance; }

		void manage(TinyRefObject* obj);
		void update();

		TinyGCInfo* getNextGCInfo();

		void collect(const TinyGCInfo* info);
	private:
		TinyGC();

	private:
		std::vector<TinyRefObject*> mMemoryPool;

		std::vector<TinyGCInfo> mGCInfos;
		std::stack<uint32_t> mFreeGCInfos;
	};

	namespace v3
	{
		class TINY_API TinyBaseRef
		{
		public:
			constexpr TinyBaseRef() noexcept = default;
			constexpr TinyBaseRef(std::nullptr_t) noexcept {}
			virtual ~TinyBaseRef()
			{
				if (mRefCount == nullptr)
				{
					return;
				}

				if (--(*mRefCount) < 1)
				{
					delete mRefCount;
					mRefCount = nullptr;
					this->release();
				}
			}

			void addRef()
			{
				(*mRefCount)++;
			}

			void subRef()
			{
				if (--(*mRefCount) < 1)
				{
					delete mRefCount;
					mRefCount = nullptr;
					delete this;
				}
			}

		protected:
			virtual void release() = 0;

		protected:
			int* mRefCount = nullptr;
		};

		template<typename Type>
		class TINY_API TinyRef : public TinyBaseRef
		{
			using PointerType = std::remove_extent_t<Type>;
		public:
			constexpr TinyRef() noexcept = default;
			constexpr TinyRef(std::nullptr_t) noexcept {}

			template<typename... Args>
			explicit TinyRef(Args&&... args) noexcept
			{
				mPointer = new PointerType(std::forward<Args>(args)...);
				mRefCount = new int();
				*mRefCount = 1;
			}

			TinyRef(const TinyRef& other)
				: mPointer(other.mPointer)
				, mRefCount(other.mRefCount)
			{
				(*mRefCount)++;
			}

			TinyRef(TinyRef&& other)
				: mPointer(other.mPointer)
				, mRefCount(other.mRefCount)
			{
				other.mPointer = nullptr;
				other.mRefCount = nullptr;
			}

			~TinyRef()
			{

			}

			void release() override
			{
				delete mPointer;
				mPointer = nullptr;
			}

		public:
			TinyRef& operator=(const TinyRef& other)
			{
				if (this == &other)
				{
					return *this;
				}

				//如果有指针
				if (mPointer != nullptr)
				{
					//计算当前指针
					if (--(*mRefCount) < 1)
					{
						delete mPointer;
						delete mRefCount;
					}
				}

				mRefCount = other.mRefCount;
				mPointer = other.mPointer;
				(*mRefCount)++;

				return *this;
			}

			TinyRef& operator=(TinyRef&& other) noexcept
			{
				mRefCount = other.mRefCount;
				mPointer = other.mPointer;

				other.mRefCount = nullptr;
				other.mPointer = nullptr;

				return *this;
			}

		public:
			PointerType* operator->() const
			{
				return mPointer;
			}

		public:
			void destory()
			{
				delete mPointer;
				mPointer = nullptr;
			}

			bool empty()
			{
				return mPointer == nullptr;
			}

			void reset(PointerType* pointer)
			{
				if (mPointer != nullptr)
				{
					abort();
				}

				mPointer = pointer;
				mRefCount = new int();
				(*mRefCount) = 1;
			}

		private:
			PointerType* mPointer = nullptr;
		};
	}


}

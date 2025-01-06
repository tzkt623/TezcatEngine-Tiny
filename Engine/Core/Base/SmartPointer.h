#pragma once

#include "../Head/CppHead.h"

namespace tezcat
{
    namespace type_tool
    {
        template<class T>
        struct Tez_PointerDeleter
        {
            constexpr Tez_PointerDeleter() noexcept = default;

            void operator()(T* pointer) const noexcept
            {
                static_assert(0 < sizeof(T), "can't delete an incomplete type");
                delete pointer;
            }
        };

        template<class T>
        struct Tez_PointerDeleter<T[]>
        {
            constexpr Tez_PointerDeleter() noexcept = default;

            void operator()(T* pointer) const noexcept
            {
                static_assert(0 < sizeof(T), "can't delete an incomplete type");
                delete[] pointer;
            }
        };

        template<class T>
        struct Tez_GetDeleter
        {
            using Deleter = Tez_PointerDeleter<T>;
        };

        template<class T>
        struct Tez_GetDeleter<T[]>
        {
            using Deleter = Tez_PointerDeleter<T[]>;
        };
    }

#pragma region Unique
    template<class T, class Deleter = type_tool::Tez_PointerDeleter<T>>
    class TezUPointer
    {
    public:
        using ElementType = typename std::remove_extent_t<T>;
        using ElementDeleter = typename type_tool::Tez_GetDeleter<ElementType>::Deleter;

    private:
        template<class U, class UDeleter>
        friend class TezUPointer;

        ElementType* mPointer{ nullptr };

    public:
        TezUPointer(const TezUPointer& other) = delete;

        TezUPointer()
        {

        }

        TezUPointer(ElementType* pointer) : mPointer(pointer)
        {

        }

        TezUPointer(TezUPointer&& other) noexcept
        {
            mPointer = std::exchange(other.mPointer, nullptr);
        }

        template<class Derived, class DerivedDeleter>
            requires(std::is_convertible_v<Derived, ElementType>)
        TezUPointer(TezUPointer<Derived, DerivedDeleter>&& other) noexcept
        {
            mPointer = static_cast<ElementType*>(std::exchange(other.mPointer, nullptr));
        }

        ~TezUPointer() noexcept
        {
            ElementDeleter{}(mPointer);
            mPointer = nullptr;
        }

        template<class Base, class BaseDeleter>
            requires(std::is_convertible_v<ElementType, Base>)
        void convertFormBaseClass(TezUPointer<Base, BaseDeleter>&& other) noexcept
        {
            if (mPointer)
            {
                ElementDeleter{}(mPointer);
            }

            mPointer = static_cast<ElementType*>(std::exchange(other.mPointer, nullptr));
        }

        template<class Derived, class DerivedDeleter>
            requires(std::is_convertible_v<Derived, ElementType>)
        void convertFromDerivedClass(TezUPointer<Derived, DerivedDeleter>&& other) noexcept
        {
            if (mPointer)
            {
                ElementDeleter{}(mPointer);
            }

            mPointer = static_cast<ElementType*>(std::exchange(other.mPointer, nullptr));
        }

    public:
        TezUPointer& operator=(const TezUPointer& other) = delete;

        TezUPointer& operator=(TezUPointer&& other) noexcept
        {
            if (this == &other)
            {
                return *this;
            }

            if (mPointer)
            {
                ElementDeleter{}(mPointer);
                mPointer = nullptr;
            }

            mPointer = std::exchange(other.mPointer, nullptr);
        }

        ElementType* operator->() const noexcept { return mPointer; }

        ElementType& operator*() const noexcept { return *mPointer; }

    public:
        ElementType* release() noexcept
        {
            return std::exchange(mPointer, nullptr);
        }

        ElementType* get() noexcept { return mPointer; }

        ElementType* get() const noexcept { return mPointer; }

        void reset(ElementType* value)
        {
            if (mPointer)
            {
                ElementDeleter{}(mPointer);
            }

            mPointer = value;
        }

        ElementDeleter& getDeleter() noexcept
        {
            return ElementDeleter{};
        }

        ElementDeleter& getDeleter() const noexcept
        {
            return ElementDeleter{};
        }
    };

    template<class T, class Deleter>
    class TezUPointer<T[], Deleter>
    {
    public:
        using ElementType = typename std::remove_extent_t<T>;
        using ElementDeleter = typename type_tool::Tez_GetDeleter<ElementType[]>::Deleter;

    private:
        template<class U, class UDeleter>
        friend class TezUPointer;

        ElementType* mPointer{ nullptr };

    public:
        TezUPointer(const TezUPointer& other) = delete;

        TezUPointer() noexcept
        {

        }

        TezUPointer(ElementType* pointer) noexcept
            : mPointer(pointer)
        {

        }

        TezUPointer(TezUPointer&& other) noexcept
        {
            mPointer = std::exchange(other.mPointer, nullptr);
        }

        /// <summary>
        /// 基类转换
        /// </summary>
        template<class Derived, class DerivedDeleter>
        requires (std::is_convertible_v<Derived, ElementType>)
        TezUPointer(TezUPointer<Derived[], DerivedDeleter>&& other) noexcept
        {
            mPointer = dynamic_cast<ElementType*>(std::exchange(other.mElement, nullptr));
        }

        ~TezUPointer() noexcept
        {
            ElementDeleter{}(mPointer);
            mPointer = nullptr;
        }

        template<class Base, class BaseDeleter>
            requires(std::is_convertible_v<ElementType, Base>)
        void convertFormBaseClass(TezUPointer<Base, BaseDeleter>&& other) noexcept
        {
            if (mPointer)
            {
                ElementDeleter{}(mPointer);
            }

            mPointer = static_cast<ElementType*>(std::exchange(other.mPointer, nullptr));
        }

        template<class Derived, class DerivedDeleter>
            requires(std::is_convertible_v<Derived, ElementType>)
        void convertFromDerivedClass(TezUPointer<Derived, DerivedDeleter>&& other) noexcept
        {
            if (mPointer)
            {
                ElementDeleter{}(mPointer);
            }

            mPointer = static_cast<ElementType*>(std::exchange(other.mPointer, nullptr));
        }

    public:
        TezUPointer& operator=(const TezUPointer& other) = delete;

        TezUPointer& operator=(TezUPointer&& other) noexcept
        {
            if (this == &other)
            {
                return *this;
            }

            if (mPointer)
            {
                Deleter{}(mPointer);
                mPointer = nullptr;
            }

            mPointer = std::exchange(other.mPointer, nullptr);
        }

        ElementType* operator->() const noexcept {
            return mPointer;
        }

        ElementType& operator*() const noexcept { return *mPointer; }

    public:
        ElementType* release() noexcept
        {
            return std::exchange(mPointer, nullptr);
        }

        ElementType* get() noexcept { return mPointer; }

        ElementType* get() const noexcept { return mPointer; }

        void reset(ElementType* value)
        {
            if (mPointer)
            {
                this->getDeleter()(mPointer);
            }

            mPointer = value;
        }

        ElementDeleter& getDeleter() noexcept
        {
            return ElementDeleter{};
        }

        ElementDeleter& getDeleter() const noexcept
        {
            return ElementDeleter{};
        }
    };
#pragma endregion

#pragma region Shared
    template<class T>
    class TezWPointer;

    struct Tez_SharedPointerBaseMetaData
    {
        int32_t mRefrenceCount{ 0 };
        int32_t mWeakRefrenceCount{ 0 };
        void* mPointer{ nullptr };


        Tez_SharedPointerBaseMetaData() = default;
        virtual ~Tez_SharedPointerBaseMetaData() = default;

        Tez_SharedPointerBaseMetaData(const Tez_SharedPointerBaseMetaData&) = delete;
        Tez_SharedPointerBaseMetaData(Tez_SharedPointerBaseMetaData&&) = delete;

        virtual void closeSharedPointer() = 0;
        virtual void closeWeakPointer() = 0;
    };

    struct Tez_SharedPointerDefaultMetaData : Tez_SharedPointerBaseMetaData
    {
        virtual void closeSharedPointer() override final
        {

        }

        virtual void closeWeakPointer() override final
        {

        }

        static Tez_SharedPointerBaseMetaData* getInstacne()
        {
            static Tez_SharedPointerDefaultMetaData instance;
            //std::cout << "SharedPointerMetaData Instacne\n";
            return &instance;
        }

    private:
        using Tez_SharedPointerBaseMetaData::Tez_SharedPointerBaseMetaData;
        Tez_SharedPointerDefaultMetaData() {}
        virtual ~Tez_SharedPointerDefaultMetaData() {}
    };

    template<class T, class Deleter>
    struct Tez_SharedPointerMetaData : public Tez_SharedPointerBaseMetaData
    {
        using Tez_SharedPointerBaseMetaData::Tez_SharedPointerBaseMetaData;

        Tez_SharedPointerMetaData() = default;
        virtual ~Tez_SharedPointerMetaData() = default;

        Tez_SharedPointerMetaData(const Tez_SharedPointerMetaData&) = delete;
        Tez_SharedPointerMetaData(Tez_SharedPointerMetaData&&) = delete;

        virtual void closeSharedPointer() override
        {
            if ((--mRefrenceCount) <= 0)
            {
                mRefrenceCount = 0;
                if (mPointer)
                {
                    Deleter{}((T*)mPointer);
                    mPointer = nullptr;
                }

                //如果还有弱引用存在,不删除元数据类
                if (mWeakRefrenceCount == 0)
                {
                    mWeakRefrenceCount = 0;
                    delete this;
                }
            }
        }

        virtual void closeWeakPointer() override
        {
            if ((--mWeakRefrenceCount) <= 0)
            {
                mWeakRefrenceCount = 0;
                //持有对象被删除了,才会删除元数据类
                if (mRefrenceCount == 0 && mWeakRefrenceCount == 0)
                {
                    delete this;
                }
            }
        }
    };

    template<class T>
    class TezSPointer
    {
    public:
        using ElementType = typename std::remove_extent_t<T>;
        using ElementDeleter = typename type_tool::Tez_GetDeleter<ElementType>::Deleter;

    private:
        template<class U>
        friend class TezSPointer;

        template<class U>
        friend class TezWPointer;

        Tez_SharedPointerBaseMetaData* mMetaData{ nullptr };

    public:
        TezSPointer()
            : mMetaData(Tez_SharedPointerDefaultMetaData::getInstacne())
        {

        }

        TezSPointer(ElementType* ptr)
            : mMetaData(new Tez_SharedPointerMetaData<ElementType, ElementDeleter>())
        {
            mMetaData->mPointer = ptr;
            mMetaData->mRefrenceCount = 1;
        }

        TezSPointer(const TezSPointer& other)
        {
            mMetaData = other.mMetaData;
            mMetaData->mRefrenceCount++;
        }

        TezSPointer(TezSPointer&& other) noexcept
        {
            mMetaData = other.mMetaData;
            other.reset();
        }

        ~TezSPointer() noexcept
        {
            mMetaData->closeSharedPointer();
            mMetaData = nullptr;
        }

    public:
        TezSPointer& operator=(const TezSPointer& other) noexcept
        {
            if (this == &other)
            {
                return *this;
            }


            mMetaData->closeSharedPointer();
            mMetaData = other.mMetaData;
            mMetaData->mRefrenceCount++;

            return *this;
        }

        TezSPointer& operator=(TezSPointer&& other) noexcept
        {
            if (this == &other)
            {
                return *this;
            }

            mMetaData = other.mMetaData;
            other.reset();

            return *this;
        }

        ElementType* operator->() const noexcept { return this->get(); }

        ElementType& operator*() const noexcept { return *this->get(); }

        explicit operator bool() const noexcept
        {
            return mMetaData->mPointer != nullptr;
        }

    public:

        /*
        * Before C++ 20
        * template<class Base, std::enable_if_t<std::is_convertible_v<ElementType, Base>, int> = 0>
        */
        template<class Base>
            requires(std::is_convertible_v<ElementType, Base>)
        void convertFromBaseClass(const TezSPointer<Base>& other) noexcept
        {
            mMetaData->closeSharedPointer();
            mMetaData = other.mMetaData;
            mMetaData->mRefrenceCount++;
        }

        template<class Derived>
            requires(std::is_convertible_v<Derived, ElementType>)
        void convertFromDerivedClass(const TezSPointer<Derived>& other) noexcept
        {
            mMetaData->closeSharedPointer();
            mMetaData = other.mMetaData;
            mMetaData->mRefrenceCount++;
        }


    public:
        ElementType* get() const noexcept { return static_cast<ElementType*>(mMetaData->mPointer); }

        auto refCount() const noexcept { return mMetaData->mRefrenceCount; }

        void reset() noexcept
        {
            mMetaData->closeSharedPointer();
            mMetaData = Tez_SharedPointerDefaultMetaData.getInstacne();
        }

        template<class U>
        void reset(U* ptr)
        {
            mMetaData->closeSharedPointer();
            mMetaData = new Tez_SharedPointerMetaData<U, type_tool::Tez_GetDeleter<U>::Deleter>();
            mMetaData->mPointer = ptr;
            mMetaData->mRefrenceCount = 1;
        }

        template<class U, class UDeleter>
        void reset(U* ptr)
        {
            mMetaData->closeSharedPointer();
            mMetaData = new Tez_SharedPointerMetaData<U, UDeleter>();
            mMetaData->mPointer = ptr;
            mMetaData->mRefrenceCount = 1;
        }

        ElementDeleter& getDeleter() noexcept
        {
            return ElementDeleter{};
        }

        ElementDeleter& getDeleter() const noexcept
        {
            return ElementDeleter{};
        }
    };

    template<class T>
    class TezSPointer<T[]>
    {
    public:
        using ElementType = typename std::remove_extent_t<T>;
        using ElementDeleter = typename type_tool::Tez_GetDeleter<ElementType[]>::Deleter;

    private:
        template<class U>
        friend class TezSPointer;

        template<class U>
        friend class TezWPointer;

        Tez_SharedPointerBaseMetaData* mMetaData{ nullptr };
        size_t mArraySize{ 0 };

    public:
        TezSPointer()
            : mMetaData(Tez_SharedPointerDefaultMetaData::getInstacne())
        {

        }

        TezSPointer(size_t size, ElementType* ptr)
            : mMetaData(new Tez_SharedPointerMetaData<ElementType, ElementDeleter>())
        {
            mMetaData->mPointer = ptr;
            mMetaData->mRefrenceCount = 1;
            mArraySize = size;
        }

        TezSPointer(const TezSPointer& other)
        {
            mMetaData = other.mMetaData;
            mMetaData->mRefrenceCount++;
            mArraySize = other.mArraySize;
        }

        TezSPointer(TezSPointer&& other) noexcept
        {
            mMetaData = other.mMetaData;
            mArraySize = other.mArraySize;

            other.reset();
        }

        ~TezSPointer() noexcept
        {
            mArraySize = 0;

            mMetaData->closeSharedPointer();
            mMetaData = nullptr;
        }

    public:
        TezSPointer& operator=(const TezSPointer& other) noexcept
        {
            if (this == &other)
            {
                return *this;
            }


            mMetaData->closeSharedPointer();
            mMetaData = other.mMetaData;
            mArraySize = other.mArraySize;

            mMetaData->mRefrenceCount++;

            return *this;
        }

        TezSPointer& operator=(TezSPointer&& other) noexcept
        {
            if (this == &other)
            {
                return *this;
            }

            mMetaData = other.mMetaData;
            mArraySize = other.mArraySize;

            other.reset();

            return *this;
        }

        ElementType* operator->() const noexcept { return this->get(); }

        ElementType& operator*() const noexcept { return *this->get(); }

        ElementType& operator[](size_t index)
        {
            if (index >= mArraySize)
            {
                throw std::out_of_range("array index out of range");
            }

            return this->get()[index];
        }

        explicit operator bool() const noexcept
        {
            return mMetaData->mPointer != nullptr;
        }

    public:
        template<class Base>
            requires(std::is_convertible_v<ElementType, Base>)
        void convertFromBaseClass(const TezSPointer<Base>& other) noexcept
        {
            mMetaData->closeSharedPointer();
            mMetaData = other.mMetaData;
            mArraySize = other.mArraySize;

            mMetaData->mRefrenceCount++;
        }

        template<class Derived>
            requires(std::is_convertible_v<Derived, ElementType>)
        void convertFromDerivedClass(const TezSPointer<Derived>& other) noexcept
        {
            mMetaData->closeSharedPointer();
            mMetaData = other.mMetaData;
            mArraySize = other.mArraySize;

            mMetaData->mRefrenceCount++;
        }


    public:
        ElementType* get() const noexcept { return static_cast<ElementType*>(mMetaData->mPointer); }

        auto refCount() const noexcept { return mMetaData->mRefrenceCount; }

        auto arraySize() const noexcept { return mArraySize; }

        void reset() noexcept
        {
            mMetaData->closeSharedPointer();
            mMetaData = Tez_SharedPointerDefaultMetaData.getInstacne();
            mArraySize = 0;
        }

        template<class U>
        void reset(size_t size, U* ptr)
        {
            mMetaData->closeSharedPointer();
            mMetaData = new Tez_SharedPointerMetaData<U, type_tool::Tez_GetDeleter<U[]>::Deleter>();
            mMetaData->mPointer = ptr;
            mMetaData->mRefrenceCount = 1;
            mArraySize = size;
        }

        template<class U, class UDeleter>
        void reset(size_t size, U* ptr)
        {
            mMetaData->closeSharedPointer();
            mMetaData = new Tez_SharedPointerMetaData<U, UDeleter>();
            mMetaData->mPointer = ptr;
            mMetaData->mRefrenceCount = 1;
            mArraySize = size;
        }

        ElementDeleter& getDeleter() noexcept
        {
            return ElementDeleter{};
        }

        ElementDeleter& getDeleter() const noexcept
        {
            return ElementDeleter{};
        }
    };
#pragma endregion Shared

#pragma region Weak
    template<class T>
    class TezWPointer
    {
        Tez_SharedPointerBaseMetaData* mMetaData{ nullptr };

    public:
        TezWPointer()
            : mMetaData(Tez_SharedPointerDefaultMetaData::getInstacne())
        {

        }

        TezWPointer(const TezSPointer<T>& sharedPointer) noexcept
        {
            mMetaData = sharedPointer.mMetaData;
            mMetaData->mWeakRefrenceCount++;
        }

        TezWPointer(const TezWPointer<T>& other) noexcept
        {
            mMetaData = other.mMetaData;
            mMetaData->mWeakRefrenceCount++;
        }

        TezWPointer(TezWPointer<T>&& other) noexcept
        {
            mMetaData = other.mMetaData;
            other.reset();
        }

        ~TezWPointer()
        {
            mMetaData->closeWeakPointer();
            mMetaData = nullptr;
        }

    public:
        TezWPointer& operator=(const TezSPointer<T>& sharedPointer) noexcept
        {
            mMetaData->closeWeakPointer();
            mMetaData = sharedPointer.mMetaData;
            mMetaData->mWeakRefrenceCount++;

            return *this;
        }

        TezWPointer& operator=(const TezWPointer<T>& other) noexcept
        {
            if (this == &other)
            {
                return *this;
            }

            mMetaData->closeWeakPointer();
            mMetaData = other.mMetaData;
            mMetaData->mWeakRefrenceCount++;

            return *this;
        }

        TezWPointer& operator=(TezWPointer<T>&& other) noexcept
        {
            if (this == &other)
            {
                return *this;
            }

            mMetaData->closeWeakPointer();
            mMetaData = other.mMetaData;
            other.reset();

            return *this;
        }

    public:
        void reset() noexcept
        {
            mMetaData->closeWeakPointer();
            mMetaData = Tez_SharedPointerDefaultMetaData::getInstacne();
        }

    public:
        bool lock() const noexcept
        {
            return mMetaData->mPointer != nullptr;
        }

        T* get() const noexcept { return static_cast<T*>(mMetaData->mPointer); }

        T* operator->() const noexcept { return this->get(); }

        T& operator*() const noexcept { return *this->get(); }
    };

    template<class T>
    class TezWPointer<T[]>
    {
        Tez_SharedPointerBaseMetaData* mMetaData{ nullptr };
        size_t mArraySize{ 0 };

    public:
        TezWPointer()
            : mMetaData(Tez_SharedPointerDefaultMetaData::getInstacne())
        {

        }

        TezWPointer(const TezSPointer<T[]>& sharedPointer) noexcept
        {
            mMetaData = sharedPointer.mMetaData;
            mMetaData->mWeakRefrenceCount++;
            mArraySize = sharedPointer.mArraySize;
        }

        TezWPointer(const TezWPointer<T[]>& other) noexcept
        {
            mMetaData = other.mMetaData;
            mArraySize = other.mArraySize;

            mMetaData->mWeakRefrenceCount++;
        }

        TezWPointer(TezWPointer<T[]>&& other) noexcept
        {
            mMetaData = other.mMetaData;
            mArraySize = other.mArraySize;

            other.reset();
        }

        ~TezWPointer()
        {
            mMetaData->closeWeakPointer();
            mArraySize = 0;
            mMetaData = nullptr;
        }

    public:
        TezWPointer& operator=(const TezSPointer<T[]>& sharedPointer) noexcept
        {
            mMetaData->closeWeakPointer();
            mMetaData = sharedPointer.mMetaData;
            mArraySize = sharedPointer.mArraySize;

            mMetaData->mWeakRefrenceCount++;

            return *this;
        }

        TezWPointer& operator=(const TezWPointer<T[]>& other) noexcept
        {
            if (this == &other)
            {
                return *this;
            }

            mMetaData->closeWeakPointer();
            mMetaData = other.mMetaData;
            mArraySize = other.mArraySize;

            mMetaData->mWeakRefrenceCount++;

            return *this;
        }

        TezWPointer& operator=(TezWPointer<T[]>&& other) noexcept
        {
            if (this == &other)
            {
                return *this;
            }

            mMetaData->closeWeakPointer();
            mMetaData = other.mMetaData;
            mArraySize = other.mArraySize;

            other.reset();

            return *this;
        }

    public:
        void reset() noexcept
        {
            mMetaData->closeWeakPointer();
            mMetaData = Tez_SharedPointerDefaultMetaData::getInstacne();
            mArraySize = 0;
        }

        size_t arraySize() const noexcept
        {
            return mArraySize;
        }

    public:
        bool lock() const noexcept
        {
            return mMetaData->mPointer != nullptr;
        }

        T* get() const noexcept { return static_cast<T*>(mMetaData->mPointer); }

        T* operator->() const noexcept { return this->get(); }

        T& operator*() const noexcept { return *this->get(); }

        T& operator[](size_t index)
        {
            if (index >= mArraySize)
            {
                throw std::out_of_range("array index out of range");
            }

            return this->get()[index];
        }
    };
#pragma endregion

}

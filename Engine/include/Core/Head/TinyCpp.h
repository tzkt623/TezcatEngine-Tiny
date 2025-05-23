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

#include "CppHead.h"
#include "../Base/TinyString.h"

namespace tezcat::Tiny
{
	using TinyUID = int32_t;
	using TinyUUID = uint32_t;

	template<class T>
	class TINY_API TinyLitArray
	{
		T* mArray = nullptr;
		uint32_t mSize = 0;

	public:
		void init(uint32_t size)
		{
			if (mArray)
			{
				delete[] mArray;
			}
			mArray = new T[size];
			mSize = size;
		}

		~TinyLitArray()
		{
			delete[] mArray;
		}

		T& operator[](uint32_t index) { return mArray[index]; }
		const T& operator[](uint32_t index) const { return mArray[index]; }

		const uint32_t& getSize() const { return mSize; }
	};

	namespace v2
	{
		class TINY_API TinyBaseRef
		{
		public:
			TinyBaseRef(const TinyBaseRef& other)
			{

			}
		};

		template<typename Type>
		class TINY_API TinyRef
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
				if (mRefCount == nullptr)
				{
					return;
				}

				if (--(*mRefCount) < 1)
				{
					delete mPointer;
					delete mRefCount;
				}
			}

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
				delete mRefCount;

				mPointer = nullptr;
				mRefCount = nullptr;
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
			int* mRefCount = nullptr;
			PointerType* mPointer = nullptr;
		};

		template<typename Base, typename Type, typename... Args>
		TinyRef<Base> make_shared(Args&&... args)
		{
			Type* p = new Type(std::forward<Args>(args)...);
			TinyRef<Type> ref;
			ref.resetForMakeShared(p);
			return ref;
		}
	}

	//---------------------------------------------
	//
	//  Stack
	//
	template<typename T>
	class TinyStack
	{
		using size_type = std::stack<T>::size_type;
		using value_type = std::stack<T>::value_type;
		using reference = std::stack<T>::reference;
		using const_reference = std::stack<T>::const_reference;

	public:


		~TinyStack()
		{
			this->clear();
		}

		void push(value_type&& value)
		{
			value->saveObject();
			mData.push(std::forward<value_type>(value));
		}

		void push(const value_type& value)
		{
			value->saveObject();
			mData.push(value);
		}

		void pop()
		{
			mData.top()->deleteObject();
			mData.pop();
		}

		constexpr reference top()
		{
			return mData.top();
		}

		constexpr size_type size() const
		{
			return mData.size();
		}

		bool empty() const
		{
			return mData.empty();
		}

		void clear()
		{
			while (!mData.empty())
			{
				mData.top()->deleteObject();
				mData.pop();
			}
		}

	private:
		std::stack<T> mData;
	};

	//---------------------------------------------
	//
	//  Vector
	//
	template<typename T>
	class TinyVector
	{
		using iterator = std::vector<T>::iterator;
		using const_iterator = std::vector<T>::const_iterator;
		using size_type = std::vector<T>::size_type;
		using value_type = std::vector<T>::value_type;
		//using reference = std::vector<T>::reference;

	public:
		~TinyVector()
		{
			this->clear();
		}

		void push_back(value_type&& obj)
		{
			obj->saveObject();
			mData.push_back(obj);
		}

		void push_back(const value_type& obj)
		{
			obj->saveObject();
			mData.push_back(obj);
		}

		template<typename... Args>
		constexpr decltype(auto) emplace_back(Args&&... args)
		{
			auto obj = mData.emplace_back(std::forward<Args>(args)...);
			obj->saveObject();
			return obj;
		}

		void clear() noexcept
		{
			for (auto& s : mData)
			{
				if (s)
				{
					s->deleteObject();
				}
			}

			mData.clear();
		}

		constexpr iterator begin() noexcept
		{
			return mData.begin();
		}

		constexpr iterator end() noexcept
		{
			return mData.end();
		}

		constexpr const_iterator begin() const noexcept
		{
			return mData.begin();
		}

		constexpr const_iterator end() const noexcept
		{
			return mData.end();
		}

		constexpr iterator erase(iterator& it) noexcept
		{
			(*it)->deleteObject();
			return mData.erase(it);
		}

		void resize(size_type size, const value_type& value)
		{
			mData.resize(size, value);
		}

		constexpr size_type size() const noexcept
		{
			return mData.size();
		}

	public:
		constexpr const T& operator[](const size_type pos) const noexcept
		{
			return mData[pos];
		}

		constexpr T& operator[](const size_type pos) noexcept
		{
			return mData[pos];
		}

	private:
		std::vector<T> mData;
	};


	//---------------------------------------------
	//
	//  List
	//
	template<typename T>
	class TinyList
	{
		using iterator = std::list<T>::iterator;
		using const_iterator = std::list<T>::const_iterator;
		using size_type = std::list<T>::size_type;
		using value_type = std::list<T>::value_type;
		using reference = std::list<T>::reference;
		using const_reference = std::list<T>::const_reference;

	public:
		~TinyList()
		{
			this->clear();
		}

		reference front() noexcept
		{
			return mData.front();
		}

		reference back() noexcept
		{
			return mData.back();
		}

		void pop_back() noexcept
		{
			mData.back()->deleteObject();
			mData.pop_back();
		}

		void pop_front() noexcept
		{
			mData.front()->deleteObject();
			mData.pop_front();
		}

		constexpr void push_back(value_type&& obj)
		{
			obj->saveObject();
			mData.push_back(obj);
		}

		constexpr void push_back(const value_type& obj)
		{
			obj->saveObject();
			mData.push_back(obj);
		}

		template<typename... Args>
		constexpr decltype(auto) emplace_back(Args&&... args)
		{
			auto obj = mData.emplace_back(std::forward<Args>(args)...);
			obj->saveObject();
			return obj;
		}

		template<typename... Args>
		constexpr decltype(auto) emplace_front(Args&&... args)
		{
			auto obj = mData.emplace_front(std::forward<Args>(args)...);
			obj->saveObject();
			return obj;
		}

		auto remove(const value_type& _Val)
		{
			return mData.remove(_Val);
		}

		template<typename Func>
		auto remove_if(Func func)
		{
			return mData.remove_if(func);
		}

		void clear()
		{
			for (auto& s : mData)
			{
				s->deleteObject();
			}

			mData.clear();
		}

		iterator begin() noexcept
		{
			return mData.begin();
		}

		iterator end() noexcept
		{
			return mData.end();
		}

		const_iterator begin() const noexcept
		{
			return mData.begin();
		}

		const_iterator end() const noexcept
		{
			return mData.end();
		}

		iterator erase(iterator& it) noexcept
		{
			(*it)->deleteObject();
			return mData.erase(it);
		}

		void resize(size_type size, const value_type& value)
		{
			mData.resize(size, value);
		}

		bool empty() const noexcept
		{
			return mData.empty();
		}

	private:
		std::list<T> mData;
	};

	//---------------------------------------------------------
	//
	// UMap
	//
	template<class Key, class Value>
	class TinyUMap
	{
		using key_type = Key;
		using size_type = std::unordered_map<Key, Value>::size_type;
		using value_type = std::unordered_map<Key, Value>::value_type;
		using reference = std::unordered_map<Key, Value>::reference;
		using const_reference = std::unordered_map<Key, Value>::const_reference;
		using iterator = std::unordered_map<Key, Value>::iterator;
		using const_iterator = std::unordered_map<Key, Value>::const_iterator;
		using mapped_type = std::unordered_map<Key, Value>::mapped_type;

		template<class T1, class T2>
		using pair = std::pair<T1, T2>;

	public:
		TinyUMap()
		{

		}
		~TinyUMap()
		{
			this->clear();
		}

		template <class Values>
		constexpr iterator insert(Values&& value)
		{
			return this->emplace(std::forward<Values>(value));
		}

		template <class Values>
		constexpr iterator insert(const_iterator where, Values&& value)
		{
			return this->emplace_hint(where, std::forward<Values>(value));
		}

		template<class... Values>
		constexpr iterator emplace_hint(const_iterator where, Values&&... args)
		{
			auto it = mData.emplace_hint(where, std::forward<Values>(args)...);
			it->second->saveObject();
			return it;
		}

		template<class... Values>
		constexpr pair<iterator, bool> emplace(Values&&... values)
		{
			auto pair = mData.emplace(std::forward<Values>(values)...);
			if (pair.second)
			{
				pair.first->second->saveObject();
			}

			return pair;
		}


		/*
		* @author HCL
		* @info 2023|5|17
		* @brief 试着加入一个元素,如果元素已经存在,则不会出发后面的创建方法
		*/
		pair<iterator, bool> tryEmplace(const key_type& key, const std::function<Value()>& function)
		{
			auto pair = mData.try_emplace(key, nullptr);
			if (pair.second)
			{
				pair.first->second = function();
				pair.first->second->saveObject();
			}

			return pair;

			//return mData.try_emplace(key, std::forward<Values>(values)...);
		}

		template<class Values>
		pair<iterator, bool> try_emplace(const key_type& key, Values&& values)
		{
			return mData.try_emplace(key, std::forward<Values>(values));
		}

		template<class Values>
		pair<iterator, bool> try_emplace(key_type&& key, Values&& values)
		{
			return mData.try_emplace(std::forward<key_type>(key), std::forward<Values>(values));
		}

		constexpr iterator find(const key_type& key)
		{
			return mData.find(key);
		}

		constexpr const_iterator find(const key_type& key) const
		{
			return mData.find(key);
		}

		constexpr iterator begin()
		{
			return mData.begin();
		}

		constexpr iterator end()
		{
			return mData.end();
		}

		constexpr const_iterator begin() const
		{
			return mData.begin();
		}

		constexpr const_iterator end() const
		{
			return mData.end();
		}

		constexpr size_type size()
		{
			return mData.size();
		}

		constexpr bool empty()
		{
			return mData.empty();
		}

		void clear()
		{
			for (auto& pair : mData)
			{
				pair.second->deleteObject();
			}

			mData.clear();
		}

	public:
		mapped_type& operator[](key_type&& value)
		{
			return mData[std::forward<key_type>(value)];
		}

		mapped_type& operator[](const key_type& value)
		{
			return mData[value];
		}

	private:
		std::unordered_map<Key, Value> mData;
	};


	//----------------------------------------------------
	//
	//	LinkedList
	//
	//
	template<class List>
	class TINY_API TinyLinkedNode
	{
		TinyLinkedNode* preNode;
		TinyLinkedNode* nextNode;

		void removeSelfFromList()
		{

		}
	};

	template<class Node>
	class TINY_API TinyLinkedNodeList
	{
		using NodeType = std::enable_if_t<std::is_base_of_v<TinyLinkedNode<TinyLinkedNodeList>, Node>& std::is_class_v<Node>, Node>;

	public:
		void pushBack(NodeType* node)
		{
			if (mFront == nullptr)
			{
				mFront = node;
				mBack = node;
			}
			else
			{
				mBack->nextNode = node;
				node->preNode = mBack;
				mBack = node;
			}
		}

		void pushFront(NodeType* node)
		{
			if (mFront == nullptr)
			{
				mFront = node;
				mBack = node;
			}
			else
			{
				mFront->preNode = node;
				node->nextNode = node;
				mFront = node;
			}
		}

	private:
		NodeType* mFront;
		NodeType* mBack;
	};
}

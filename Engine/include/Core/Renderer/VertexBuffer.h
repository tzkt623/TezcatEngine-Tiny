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

#include "VertexConfig.h"
#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"
#include "../Head/GLMHead.h"
#include "../Renderer/RenderConfig.h"
#include "../Base/TinyObject.h"

namespace tezcat::Tiny
{
	class TINY_API IBuffer : public TinyObject
	{
		TINY_ABSTRACT_OBJECT_H(IBuffer, TinyObject)

	protected:
		IBuffer();

	protected:
		virtual void init() override { }

	public:
		virtual ~IBuffer();

		void init(const size_t& dataSize, const void* data = nullptr);

		void updateData(const void* data, const size_t& dataSize);

		void resetSize(size_t size);

		void copy(const void* data, const size_t& dataSize, const size_t& offset);

		void apply(uint32_t id)
		{
			mBufferID = id;
			mGenerated = true;
		}
		const uint32_t& getBufferID() const { return mBufferID; }

		const void* getData() const { return mData; }

		uint64_t getDataSize() const { return mDataSize; }

		void release() { free(mData); }

		const bool isGenerated() const { return mGenerated; }

	protected:
		uint32_t mBufferID = 0;
		uint64_t mDataSize;
		void* mData;
		bool mGenerated;
	};

	/// <summary>
	/// 对应gl的VBO
	/// </summary>
	class TINY_API VertexBuffer : public IBuffer
	{
		VertexBuffer();
		TINY_OBJECT_H(VertexBuffer, IBuffer)

	public:
		virtual ~VertexBuffer();

		void setLayoutData(VertexPosition position);
		const VertexLayoutData& getLayoutData() const { return mLayoutData; }

	protected:
		void onClose() override;

	protected:
		VertexLayoutData mLayoutData;
	};


	class TINY_API IndexBuffer : public IBuffer
	{
		IndexBuffer();
		TINY_OBJECT_H(IndexBuffer, IBuffer)

	public:
		virtual ~IndexBuffer();

	protected:
		void onClose() override;
	};


	/// <summary>
	/// UniformBuffer的使用场景
	///
	/// 1.Camera的数据
	/// 保存投影矩阵,视图矩阵,相机远近平面,相机世界坐标
	///
	/// 2.Skybox的数据
	/// 投影矩阵,6个视图矩阵,贴图
	/// 
	/// </summary>
	class TINY_API UniformBuffer : public IBuffer
	{
		UniformBuffer();
		TINY_OBJECT_H(UniformBuffer, IBuffer)

	public:
		virtual ~UniformBuffer();

		template<class Config>
		void setLayout(const std::function<void(UniformBufferLayout*)>& function)
		{
			this->setLayout(Config::Name, function);
		}

		/*
		* 设置一个Layout
		* 如果已经设置过的,将跳过设置函数
		*/
		void setLayout(const std::string_view& layoutName, const std::function<void(UniformBufferLayout*)>& function);

		void update(const uint32_t& index, const void* data, const size_t& dataSize);

		/*
		* 真实要填充的数据大小
		* 不用管对齐内容
		*/
		template<class Data>
		void update(const uint32_t& index, const void* data)
		{
			this->update(index, data, sizeof(Data));
		}

		template<class Config>
		void updateWithConfig(const void* data)
		{
			this->update(Config::Index, data, sizeof(Config::TrueSize));
		}

		const std::shared_ptr<UniformBufferLayout> &getLayout() { return mLayout; }

		void updateLayoutData();

	protected:
		void onClose() override;

	private:
		std::shared_ptr<UniformBufferLayout> mLayout;

	public:
		std::string mName;
		std::function<void(UniformBuffer*)> mOnLayoutDataUpdated;
	};
}

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
#include "../Head/GLMHead.h"
#include "../Renderer/RenderConfig.h"
#include "../Tool/Tool.h"

namespace tezcat::Tiny
{
	class Texture;
	class Texture2D;
	class TextureCube;
	class Shader;
	class Transform;

	struct TINY_API UniformIDDef {};
	using UniformID = IDString<UniformIDDef>;


	struct TINY_API Uniform
	{
		/*
		* @brief 传指针是为了避免shader的创建和生成不同步的情况
		* @brief 直接使用id地址可以动态生成
		*/
		Uniform(int32_t* valueID) : mValueID(valueID) {}

		virtual ~Uniform() {}

		virtual UniformType getType() = 0;
		virtual void submit(Shader* shader) = 0;

		int32_t* mValueID;
	};


	template<typename UniformValue>
	struct TINY_API UniformT : public Uniform
	{
		using ValueType = UniformValue;

		UniformT(int32_t* valueID)
			: Uniform(valueID)
			, value()
		{}
		virtual ~UniformT() {}

		void set(ValueType&& value)
		{
			this->beginSetValue();
			this->value = std::forward<ValueType>(value);
			this->endSetValue();
		}

		void set(ValueType& value)
		{
			this->beginSetValue();
			this->value = value;
			this->endSetValue();
		}

		ValueType value;

	protected:
		virtual void beginSetValue() {}
		virtual void endSetValue() {}
	};

	struct TINY_API UniformI1 : public UniformT<int32_t>
	{
		using UniformT<int32_t>::UniformT;

		UniformType getType() final { return UniformType::Int; }
		void submit(Shader* shader) override;
	};

	struct TINY_API UniformF1 : public UniformT<float>
	{
		using UniformT<float>::UniformT;

		UniformType getType() final { return UniformType::Float; }
		void submit(Shader* shader) override;
	};

	struct TINY_API UniformI2 : public UniformT<int2>
	{
		using UniformT<int2>::UniformT;

		UniformType getType() final { return UniformType::Int2; }
		void submit(Shader* shader) override;
	};

	struct TINY_API UniformF2 : public UniformT<float2>
	{
		using UniformT<float2>::UniformT;

		UniformType getType() final { return UniformType::Float2; }
		void submit(Shader* shader) override;
	};

	struct TINY_API UniformI3 : public UniformT<int3>
	{
		using UniformT<int3>::UniformT;

		UniformType getType() final { return UniformType::Int3; }
		void submit(Shader* shader) override;
	};

	struct TINY_API UniformF3 : public UniformT<float3>
	{
		using UniformT<float3>::UniformT;

		UniformType getType() final { return UniformType::Float3; }
		void submit(Shader* shader) override;
	};

	struct TINY_API UniformI4 : public UniformT<int4>
	{
		using UniformT<int4>::UniformT;

		UniformType getType() final { return UniformType::Int4; }
		void submit(Shader* shader) override;
	};

	struct TINY_API UniformF4 : public UniformT<float4>
	{
		using UniformT<float4>::UniformT;

		UniformType getType() final { return UniformType::Float4; }
		void submit(Shader* shader) override;
	};

	struct TINY_API UniformMat3 : public UniformT<float3x3>
	{
		using UniformT<float3x3>::UniformT;

		UniformType getType() final { return UniformType::Mat3; }
		void submit(Shader* shader) override;
	};

	struct TINY_API UniformMat4 : public UniformT<float4x4>
	{
		using UniformT<float4x4>::UniformT;

		UniformType getType() final { return UniformType::Mat4; }
		void submit(Shader* shader) override;
	};

	struct TINY_API UniformTex2D : public UniformT<Texture2D*>
	{
		using UniformT<Texture2D*>::UniformT;
		virtual ~UniformTex2D();


		UniformType getType() final { return UniformType::Tex2D; }
		void submit(Shader* shader) override;

	protected:
		void beginSetValue() override;
		void endSetValue() override;
	};

	struct TINY_API UniformTexCube : public UniformT<TextureCube*>
	{
		using UniformT<TextureCube*>::UniformT;
		virtual ~UniformTexCube();


		UniformType getType() final { return UniformType::TexCube; }
		void submit(Shader* shader) override;

	protected:
		void beginSetValue() override;
		void endSetValue() override;
	};
}

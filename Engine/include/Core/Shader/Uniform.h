#pragma once
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

	//struct TINY_API UniformIDDef {};


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

	using UniformID = IDString<Uniform>;

	template<typename UniformValue>
	struct TINY_API UniformValueT : public Uniform
	{
		using ValueType = UniformValue;

		UniformValueT(int32_t* valueID)
			: Uniform(valueID)
			, value()
		{}
		virtual ~UniformValueT() {}

		void set(ValueType& value)
		{
			this->value = value;
		}

		void set(ValueType&& value)
		{
			this->value = TINY_FWD(value);
		}

		ValueType value;
	};

	template<typename UniformTinyObject>
	struct TINY_API UniformTinyObjectT : public Uniform
	{
		using ValueType = UniformTinyObject;

		UniformTinyObjectT(int32_t* valueID)
			: Uniform(valueID)
			, value()
		{}
		virtual ~UniformTinyObjectT() {}

		void set(ValueType&& value)
		{
			if (this->value)
			{
				this->value->deleteObject();
			}
			this->value = TINY_FWD(value);
			if (this->value)
			{
				this->value->saveObject();
			}
		}

		void set(ValueType& value)
		{
			if (this->value)
			{
				this->value->deleteObject();
			}
			this->value = value;
			if (this->value)
			{
				this->value->saveObject();
			}
		}

		ValueType value;
	};


	//------------------------------
	//
	//	Values
	//
	struct TINY_API UniformI1 : public UniformValueT<int32_t>
	{
		using UniformValueT<int32_t>::UniformValueT;

		UniformType getType() final { return UniformType::Int; }
		void submit(Shader* shader) override;
	};

	struct TINY_API UniformF1 : public UniformValueT<float>
	{
		using UniformValueT<float>::UniformValueT;

		UniformType getType() final { return UniformType::Float; }
		void submit(Shader* shader) override;
	};

	struct TINY_API UniformI2 : public UniformValueT<int2>
	{
		using UniformValueT<int2>::UniformValueT;

		UniformType getType() final { return UniformType::Int2; }
		void submit(Shader* shader) override;
	};

	struct TINY_API UniformF2 : public UniformValueT<float2>
	{
		using UniformValueT<float2>::UniformValueT;

		UniformType getType() final { return UniformType::Float2; }
		void submit(Shader* shader) override;
	};

	struct TINY_API UniformI3 : public UniformValueT<int3>
	{
		using UniformValueT<int3>::UniformValueT;

		UniformType getType() final { return UniformType::Int3; }
		void submit(Shader* shader) override;
	};

	struct TINY_API UniformF3 : public UniformValueT<float3>
	{
		using UniformValueT<float3>::UniformValueT;

		UniformType getType() final { return UniformType::Float3; }
		void submit(Shader* shader) override;
	};

	struct TINY_API UniformI4 : public UniformValueT<int4>
	{
		using UniformValueT<int4>::UniformValueT;

		UniformType getType() final { return UniformType::Int4; }
		void submit(Shader* shader) override;
	};

	struct TINY_API UniformF4 : public UniformValueT<float4>
	{
		using UniformValueT<float4>::UniformValueT;

		UniformType getType() final { return UniformType::Float4; }
		void submit(Shader* shader) override;
	};

	struct TINY_API UniformMat3 : public UniformValueT<float3x3>
	{
		using UniformValueT<float3x3>::UniformValueT;

		UniformType getType() final { return UniformType::Mat3; }
		void submit(Shader* shader) override;
	};

	struct TINY_API UniformMat4 : public UniformValueT<float4x4>
	{
		using UniformValueT<float4x4>::UniformValueT;

		UniformType getType() final { return UniformType::Mat4; }
		void submit(Shader* shader) override;
	};

	//------------------------------
	//
	//	Objects
	//
	struct TINY_API UniformTex2D : public UniformTinyObjectT<Texture2D*>
	{
		using UniformTinyObjectT<Texture2D*>::UniformTinyObjectT;
		virtual ~UniformTex2D();


		UniformType getType() final { return UniformType::Tex2D; }
		void submit(Shader* shader) override;
	};

	struct TINY_API UniformTexCube : public UniformTinyObjectT<TextureCube*>
	{
		using UniformTinyObjectT<TextureCube*>::UniformTinyObjectT;
		virtual ~UniformTexCube();


		UniformType getType() final { return UniformType::TexCube; }
		void submit(Shader* shader) override;
	};

	template<class TName, uint32_t TIndex>
	struct U
	{
		constexpr static uint32_t TypeIndex = TIndex;
		void* data = nullptr;
	};


	struct UF1 : U<float, 0>
	{

	};
}

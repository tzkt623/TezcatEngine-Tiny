#pragma once

#include "../Head/GLMHead.h"
#include "../Head/RenderConfig.h"
#include "../Tool/Tool.h"

namespace tezcat::Tiny
{
	class Texture;
	class Texture2D;
	class TextureCube;
	class Shader;
	class Transform;
	class BaseGraphics;

	struct TINY_API UniformIDDef {};
	using UniformID = IDString<UniformIDDef>;


	struct TINY_API Uniform
	{
		/*
		* @brief 传指针是为了避免shader的创建和生成不同步的情况
		* @brief 直接使用id地址可以动态生成
		*/
		Uniform(int* shaderID) : mShaderID(shaderID) {}

		virtual ~Uniform() {}

		virtual UniformType getType() = 0;
		virtual void submit(BaseGraphics* graphics, Shader* shader) = 0;

		int* mShaderID;
	};


	template<typename UniformValue>
	struct TINY_API UniformT : public Uniform
	{
		using ValueType = UniformValue;

		UniformT(int* shaderID)
			: Uniform(shaderID)
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

	struct TINY_API UniformI1 : public UniformT<int>
	{
		using UniformT<int>::UniformT;

		UniformType getType() final { return UniformType::Int; }
		void submit(BaseGraphics* graphics, Shader* shader) override;
	};

	struct TINY_API UniformF1 : public UniformT<float>
	{
		using UniformT<float>::UniformT;

		UniformType getType() final { return UniformType::Float; }
		void submit(BaseGraphics* graphics, Shader* shader) override;
	};

	struct TINY_API UniformI2 : public UniformT<glm::ivec2>
	{
		using UniformT<glm::ivec2>::UniformT;

		UniformType getType() final { return UniformType::Int2; }
		void submit(BaseGraphics* graphics, Shader* shader) override;
	};

	struct TINY_API UniformF2 : public UniformT<glm::vec2>
	{
		using UniformT<glm::vec2>::UniformT;

		UniformType getType() final { return UniformType::Float2; }
		void submit(BaseGraphics* graphics, Shader* shader) override;
	};

	struct TINY_API UniformI3 : public UniformT<glm::ivec3>
	{
		using UniformT<glm::ivec3>::UniformT;

		UniformType getType() final { return UniformType::Int3; }
		void submit(BaseGraphics* graphics, Shader* shader) override;
	};

	struct TINY_API UniformF3 : public UniformT<glm::vec3>
	{
		using UniformT<glm::vec3>::UniformT;

		UniformType getType() final { return UniformType::Float3; }
		void submit(BaseGraphics* graphics, Shader* shader) override;
	};

	struct TINY_API UniformI4 : public UniformT<glm::ivec4>
	{
		using UniformT<glm::ivec4>::UniformT;

		UniformType getType() final { return UniformType::Int4; }
		void submit(BaseGraphics* graphics, Shader* shader) override;
	};

	struct TINY_API UniformF4 : public UniformT<glm::vec4>
	{
		using UniformT<glm::vec4>::UniformT;

		UniformType getType() final { return UniformType::Float4; }
		void submit(BaseGraphics* graphics, Shader* shader) override;
	};

	struct TINY_API UniformMat3 : public UniformT<glm::mat3>
	{
		using UniformT<glm::mat3>::UniformT;

		UniformType getType() final { return UniformType::Mat3; }
		void submit(BaseGraphics* graphics, Shader* shader) override;
	};

	struct TINY_API UniformMat4 : public UniformT<glm::mat4>
	{
		using UniformT<glm::mat4>::UniformT;

		UniformType getType() final { return UniformType::Mat4; }
		void submit(BaseGraphics* graphics, Shader* shader) override;
	};

	struct TINY_API UniformTex2D : public UniformT<Texture2D*>
	{
		using UniformT<Texture2D*>::UniformT;
		virtual ~UniformTex2D();


		UniformType getType() final { return UniformType::Tex2D; }
		void submit(BaseGraphics* graphics, Shader* shader) override;

	protected:
		void beginSetValue() override;
		void endSetValue() override;
	};

	struct TINY_API UniformTexCube : public UniformT<TextureCube*>
	{
		using UniformT<TextureCube*>::UniformT;
		virtual ~UniformTexCube();


		UniformType getType() final { return UniformType::TexCube; }
		void submit(BaseGraphics* graphics, Shader* shader) override;

	protected:
		void beginSetValue() override;
		void endSetValue() override;
	};
}

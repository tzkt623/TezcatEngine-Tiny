#pragma once

#include "../Head/GLMHead.h"
#include "../Head/Context.h"
#include "Utility/Utility.h"

namespace tezcat::Tiny::Core
{
	class Texture;
	class Texture2D;
	class TextureCube;
	class Shader;
	class Transform;

	struct TINY_API UniformIDDef {};
	using UniformID = IDString<UniformIDDef>;

	enum class TINY_API UniformType : uint32_t
	{
		Bool,
		Int, Float,
		Int2, Float2,
		Int3, Float3,
		Int4, Float4,
		Mat3, Mat4, MatNormal,
		Tex2D, Tex3D, TexCube,

		Mode,
	};


	struct TINY_API Uniform
	{
		Uniform(const UniformID& id) : ID(id) {}
		virtual ~Uniform(){}

		UniformID ID;
		inline virtual UniformType getType() = 0;
		virtual void submit(Transform* transform, Shader* shader) = 0;
	};


	template<typename UniformValue>
	struct TINY_API UniformT : public Uniform
	{
		UniformT()
		{}
		UniformT(const UniformID& id, UniformValue&& value)
			: Uniform(id)
			, value(std::forward<UniformValue>(value))
		{}
		virtual ~UniformT() {}

		UniformValue value;
	};

	struct TINY_API UniformI1 : public UniformT<int>
	{
		using UniformT<int>::UniformT;

		inline UniformType getType() final { return UniformType::Int; }
		void submit(Transform* transform, Shader* shader) override;
	};

	struct TINY_API UniformF1 : public UniformT<float>
	{
		using UniformT<float>::UniformT;

		inline UniformType getType() final { return UniformType::Float; }
		void submit(Transform* transform, Shader* shader) override;
	};

	struct TINY_API UniformI2 : public UniformT<glm::ivec2>
	{
		using UniformT<glm::ivec2>::UniformT;

		inline UniformType getType() final { return UniformType::Int2; }
		void submit(Transform* transform, Shader* shader) override;
	};

	struct TINY_API UniformF2 : public UniformT<glm::vec2>
	{
		using UniformT<glm::vec2>::UniformT;

		inline UniformType getType() final { return UniformType::Float2; }
		void submit(Transform* transform, Shader* shader) override;
	};

	struct TINY_API UniformI3 : public UniformT<glm::ivec3>
	{
		using UniformT<glm::ivec3>::UniformT;

		inline UniformType getType() final { return UniformType::Int3; }
		void submit(Transform* transform, Shader* shader) override;
	};

	struct TINY_API UniformF3 : public UniformT<glm::vec3>
	{
		using UniformT<glm::vec3>::UniformT;

		inline UniformType getType() final { return UniformType::Float3; }
		void submit(Transform* transform, Shader* shader) override;
	};

	struct TINY_API UniformI4 : public UniformT<glm::ivec4>
	{
		using UniformT<glm::ivec4>::UniformT;

		inline UniformType getType() final { return UniformType::Int4; }
		void submit(Transform* transform, Shader* shader) override;
	};

	struct TINY_API UniformF4 : public UniformT<glm::vec4>
	{
		using UniformT<glm::vec4>::UniformT;

		inline UniformType getType() final { return UniformType::Float4; }
		void submit(Transform* transform, Shader* shader) override;
	};

	struct TINY_API UniformMat3 : public UniformT<glm::mat3>
	{
		using UniformT<glm::mat3>::UniformT;

		inline UniformType getType() final { return UniformType::Mat3; }
		void submit(Transform* transform, Shader* shader) override;
	};

	struct TINY_API UniformMat4 : public UniformT<glm::mat4>
	{
		using UniformT<glm::mat4>::UniformT;

		inline UniformType getType() final { return UniformType::Mat4; }
		void submit(Transform* transform, Shader* shader) override;
	};

	struct TINY_API UniformTex2D : public UniformT<Texture2D*>
	{
		using UniformT<Texture2D*>::UniformT;
		UniformTex2D(const UniformID& id, const std::string& name);


		inline UniformType getType() final { return UniformType::Tex2D; }
		void submit(Transform* transform, Shader* shader) override;
	};

	struct TINY_API UniformTexCube : public UniformT<TextureCube*>
	{
		using UniformT<TextureCube*>::UniformT;
		UniformTexCube(const UniformID& id, const std::string & name);


		inline UniformType getType() final { return UniformType::TexCube; }
		void submit(Transform* transform, Shader* shader) override;
	};


	//-----------------------------------------------
	//
	//	特化Uniform
	//
	struct TINY_API UniformMatrixP : public Uniform
	{
		using Uniform::Uniform;

		inline UniformType getType() final { return UniformType::Mat4; }
		void submit(Transform* transform, Shader* shader) override;
	};

	struct TINY_API UniformMatrixV : public Uniform
	{
		using Uniform::Uniform;

		inline UniformType getType() final { return UniformType::Mat4; }
		void submit(Transform* transform, Shader* shader) override;
	};

	struct TINY_API UniformMatrixM : public Uniform
	{
		using Uniform::Uniform;

		inline UniformType getType() final { return UniformType::Mat4; }
		void submit(Transform* transform, Shader* shader) override;
	};

	struct TINY_API UniformMatrixN : public Uniform
	{
		using Uniform::Uniform;

		inline UniformType getType() final { return UniformType::Mat3; }
		void submit(Transform* transform, Shader* shader) override;
	};

	struct TINY_API UniformMode : public Uniform
	{
		using Uniform::Uniform;

		inline UniformType getType() final { return UniformType::Mode; }
		void submit(Transform* transform, Shader* shader) override;
	};
}

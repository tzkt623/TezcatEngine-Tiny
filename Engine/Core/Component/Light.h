#pragma once
#include "../Renderer/RenderObject.h"

#include "../Head/CppHead.h"
#include "../Head/GLMHead.h"
#include "../Shader/Shader.h"
#include "../Head/ConfigHead.h"

namespace tezcat::Tiny::Core
{
	enum class TINY_API LightType
	{
		Directional,
		Point,
		Spot
	};

	class Shader;
	class TINY_API ILight
	{
	public:
		virtual ~ILight() = default;
		virtual LightType getLightType() = 0;
		virtual void submit(Shader* shader) = 0;
		virtual bool cullGameObject(GameObject* gameObject) = 0;
	};

// 	class TINY_API Light : public ComponentT<Light>, public ILight
// 	{
// 	public:
// 		Light() = default;
// 		virtual ~Light() = default;
// 	};

	class TINY_API DirectionalLight : public ComponentT<DirectionalLight>, public ILight
	{
	public:
		DirectionalLight();
		DirectionalLight(const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular);
		virtual ~DirectionalLight();

		LightType getLightType() final { return LightType::Directional; }

		void onEnable() override;
		void onDisable() override;

	public:
		virtual bool cullGameObject(GameObject* gameObject) { return true; }
		void submit(Shader* shader) override;

		glm::vec3& getDirection() { return mDirection; }
		void setDirection(const glm::vec3& val) { mDirection = val; }

		glm::vec3& getAmbient() { return mAmbient; }
		void setAmbient(const glm::vec3& val) { mAmbient = val; }

		glm::vec3& getDiffuse() { return mDiffuse; }
		void setDiffuse(const glm::vec3& val) { mDiffuse = val; }

		glm::vec3& getSpecular() { return mSpecular; }
		void setSpecular(const glm::vec3& val) { mSpecular = val; }

	private:
		glm::vec3 mDirection;
		glm::vec3 mAmbient;
		glm::vec3 mDiffuse;
		glm::vec3 mSpecular;
	};


	class TINY_API PointLight : public ComponentT<PointLight>, public ILight
	{
	public:
		PointLight();
		virtual ~PointLight();

		LightType getLightType() final { return LightType::Point; }

		glm::vec3& getAmbient() { return mAmbient; }
		void setAmbient(const glm::vec3& val) { mAmbient = val; }

		glm::vec3& getDiffuse() { return mDiffuse; }
		void setDiffuse(const glm::vec3& val) { mDiffuse = val; }

		glm::vec3& getSpecular() { return mSpecular; }
		void setSpecular(const glm::vec3& val) { mSpecular = val; }

		glm::vec3& getConfig() { return mSpecular; }
		void setConfig(const glm::vec3& val) { mSpecular = val; }

		void setConfig(float constant, float linear, float quadratic)
		{
			mSpecular.x = constant;
			mSpecular.y = linear;
			mSpecular.z = quadratic;
		}

		void setConstant(float constant)
		{
			mSpecular.x = constant;
		}

		void setLinear(float linear)
		{
			mSpecular.y = linear;
		}

		void setQuadratic(float quadratic)
		{
			mSpecular.z = quadratic;
		}

		void submit(Shader* shader) override;
		void onEnable() override;
		void onDisable() override;
		bool cullGameObject(GameObject* gameObject) override;

	private:
		glm::vec3 mAmbient;
		glm::vec3 mDiffuse;
		glm::vec3 mSpecular;
		glm::vec3 mConfig;
	};

	class TINY_API SpotLight : public ComponentT<SpotLight>, public ILight
	{
	public:
		SpotLight();
		virtual ~SpotLight();

		LightType getLightType() final { return LightType::Spot; }
		bool cullGameObject(GameObject* gameObject) override { return true; }

	private:

	};
}

#pragma once
#include "../Renderer/RenderObject.h"

#include "../Head/CppHead.h"
#include "../Shader/Shader.h"

namespace tezcat::Tiny
{
	enum class TINY_API LightType
	{
		Directional,
		Point,
		Spot
	};

	class Shader;
	class FrameBuffer;
	class BaseGraphics;

	class TINY_API ILight : public IRenderObject
	{
	public:
		virtual ~ILight() = default;
		virtual LightType getLightType() = 0;
		RenderObjectType getRenderObjectType() final { return RenderObjectType::Light; }
		virtual void render(BaseGraphics* graphics) {}
	};

	class TINY_API DirectionalLight : public ComponentT<DirectionalLight>, public ILight
	{
		DirectionalLight();
		DirectionalLight(const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular);
	public:
		TINY_Factory(DirectionalLight);
		TINY_RTTI_H(DirectionalLight);

	public:
		virtual ~DirectionalLight();

		LightType getLightType() final { return LightType::Directional; }

	public:
		void render(BaseGraphics* graphics) override;
		void submit(Shader* shader) override;

		glm::vec3& getDirection() { return mDirection; }
		void setDirection(const glm::vec3& val) { mDirection = val; }

		glm::vec3& getAmbient() { return mAmbient; }
		void setAmbient(const glm::vec3& val) { mAmbient = val; }

		glm::vec3& getDiffuse() { return mDiffuse; }
		void setDiffuse(const glm::vec3& val) { mDiffuse = val; }

		glm::vec3& getSpecular() { return mSpecular; }
		void setSpecular(const glm::vec3& val) { mSpecular = val; }

	protected:
		void onEnable() override;
		void onDisable() override;

	private:
		glm::vec3 mDirection;
		glm::vec3 mAmbient;
		glm::vec3 mDiffuse;
		glm::vec3 mSpecular;
	};


	class TINY_API PointLight : public ComponentT<PointLight>, public ILight
	{
		PointLight();
		TINY_Factory(PointLight);
		TINY_RTTI_H(PointLight);

	public:
		virtual ~PointLight();

		LightType getLightType() final { return LightType::Point; }

		glm::vec3& getAmbient() { return mAmbient; }
		void setAmbient(const glm::vec3& val) { mAmbient = val; }

		glm::vec3& getDiffuse() { return mDiffuse; }
		void setDiffuse(const glm::vec3& val) { mDiffuse = val; }

		glm::vec3& getSpecular() { return mSpecular; }
		void setSpecular(const glm::vec3& val) { mSpecular = val; }

		glm::vec3& getConfig() { return mConfig; }
		void setConfig(const glm::vec3& val) { mConfig = val; }

		void setConfig(float constant, float linear, float quadratic)
		{
			mConfig.x = constant;
			mConfig.y = linear;
			mConfig.z = quadratic;
		}

		void setConstant(float constant)
		{
			mConfig.x = constant;
		}

		void setLinear(float linear)
		{
			mConfig.y = linear;
		}

		void setQuadratic(float quadratic)
		{
			mConfig.z = quadratic;
		}

		void submit(Shader* shader) override;

	protected:
		void onEnable() override;
		void onDisable() override;

	private:
		glm::vec3 mAmbient;
		glm::vec3 mDiffuse;
		glm::vec3 mSpecular;
		glm::vec3 mConfig;

	};

	class TINY_API SpotLight : public ComponentT<SpotLight>, public ILight
	{
		SpotLight();
		TINY_Factory(SpotLight);
		TINY_RTTI_H(SpotLight);
	public:
		virtual ~SpotLight();

		LightType getLightType() final { return LightType::Spot; }

	private:

	};
}

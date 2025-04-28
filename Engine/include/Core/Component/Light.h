#pragma once
#include "../Renderer/Renderer.h"

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

#pragma region Agent


	class TINY_API ILight : public BaseRenderer
	{
		TINY_ABSTRACT_OBJECT_H(ILight, BaseRenderer);
	public:
		virtual ~ILight() = default;
		virtual LightType getLightType() = 0;
		RenderObjectType getRenderObjectType() final { return RenderObjectType::Light; }
		virtual void render(BaseGraphics* graphics) {}
	};

	class TINY_API DirectionalLightAgent : public ILight
	{
		DirectionalLightAgent();
		TINY_OBJECT_H(DirectionalLightAgent, ILight);

	public:
		virtual ~DirectionalLightAgent() {}

		virtual LightType getLightType() final { return LightType::Directional; }

		float3& getDirection() { return mDirection; }
		void setDirection(const float3& val) { mDirection = val; }

		float3& getAmbient() { return mAmbient; }
		void setAmbient(const float3& val) { mAmbient = val; }

		float3& getDiffuse() { return mDiffuse; }
		void setDiffuse(const float3& val) { mDiffuse = val; }

		float3& getSpecular() { return mSpecular; }
		void setSpecular(const float3& val) { mSpecular = val; }

		virtual void submit(Shader* shader) override;


		void makeRenderCommand(PipelineQueue* queue) override;
		void makeRenderCommand(ReplacedPipelinePass* pass) override;

	private:
		float3 mDirection;
		float3 mAmbient;
		float3 mDiffuse;
		float3 mSpecular;
	};
#pragma endregion


#pragma region Component


	class TINY_API LightComponent : public ComponentAutoID<LightComponent>
	{
		TINY_ABSTRACT_OBJECT_H(LightComponent, ComponentAutoID<LightComponent>);

	protected:
		LightComponent() = default;

	public:
		virtual ~LightComponent() = default;
		virtual void submit(Shader* shader) = 0;
		virtual LightType getLightType() = 0;
	};

	class TINY_API DirectionalLight : public LightComponent
	{
		DirectionalLight();
		DirectionalLight(const float3& direction, const float3& ambient, const float3& diffuse, const float3& specular);
		TINY_OBJECT_H(DirectionalLight, LightComponent);

	public:
		virtual ~DirectionalLight();

		LightType getLightType() final { return LightType::Directional; }

	public:
		void render(BaseGraphics* graphics);
		virtual void submit(Shader* shader) override;

		float3& getDirection() { return mLightAgent->getDirection(); }
		void setDirection(const float3& val) { mLightAgent->setDirection(val); }

		float3& getAmbient() { return mLightAgent->getAmbient(); }
		void setAmbient(const float3& val) { mLightAgent->setAmbient(val); }

		float3& getDiffuse() { return mLightAgent->getDiffuse(); }
		void setDiffuse(const float3& val) { mLightAgent->setDiffuse(val); }

		float3& getSpecular() { return mLightAgent->getSpecular(); }
		void setSpecular(const float3& val) { mLightAgent->setSpecular(val); }

	protected:
		void onEnable() override;
		void onDisable() override;
		void onStart() override;

	private:
		DirectionalLightAgent* mLightAgent;
	};


	class TINY_API PointLight : public LightComponent
	{
		PointLight();
		TINY_OBJECT_H(PointLight, LightComponent)

	public:
		virtual ~PointLight();

		LightType getLightType() final { return LightType::Point; }

		float3& getAmbient() { return mAmbient; }
		void setAmbient(const float3& val) { mAmbient = val; }

		float3& getDiffuse() { return mDiffuse; }
		void setDiffuse(const float3& val) { mDiffuse = val; }

		float3& getSpecular() { return mSpecular; }
		void setSpecular(const float3& val) { mSpecular = val; }

		float3& getConfig() { return mConfig; }
		void setConfig(const float3& val) { mConfig = val; }

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

		virtual void submit(Shader* shader) override;


	protected:
		void onEnable() override;
		void onDisable() override;

	private:
		float3 mAmbient;
		float3 mDiffuse;
		float3 mSpecular;
		float3 mConfig;
	};

	class TINY_API SpotLight : public LightComponent
	{
		SpotLight();
		TINY_OBJECT_H(SpotLight, LightComponent)
	public:
		virtual ~SpotLight();

		LightType getLightType() final { return LightType::Spot; }
		virtual void submit(Shader* shader) override;

	private:

	};
#pragma endregion

}

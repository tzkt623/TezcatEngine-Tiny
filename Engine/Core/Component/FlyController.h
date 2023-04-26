#pragma once

#include "Component.h"
#include "../Head/GLMHead.h"
#include "../Head/ConfigHead.h"

namespace tezcat::Tiny
{
	class Transform;
	class TINY_API FlyController : public ComponentT<FlyController>
	{
		FlyController();
	public:
		TINY_Factory(FlyController)
		TINY_RTTI_H(FlyController)

	public:
		virtual ~FlyController();
		void yawPitch(float yaw, float pitch, bool constrainPitch = true);
		void translate(const glm::vec3& offset);
		void roll(float roll);
		void updateVector(Transform* transform);

		glm::vec3 getForward(); 
		glm::vec3 getUp(); 
		glm::vec3 getRight();

	protected:
		void onStart() override;
		void onEnable() override;


	private:
		glm::vec3 mFront;
		glm::vec3 mUp;
		glm::vec3 mRight;
		float mYaw;
		float mPitch;
		float mRoll;

		static const glm::vec3 sWorldUp;
	};
}


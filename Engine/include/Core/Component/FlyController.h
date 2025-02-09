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
		TINY_OBJECT_H(FlyController, ComponentT<FlyController>)

	public:
		virtual ~FlyController();
		void yawPitch(float yaw, float pitch, bool constrainPitch = true);
		void translate(const float3& offset);
		void roll(float roll);
		void updateVector(Transform* transform);

		float3 getForward(); 
		float3 getUp(); 
		float3 getRight();

	protected:
		void onStart() override;
		void onEnable() override;


	private:
		float3 mFront;
		float3 mUp;
		float3 mRight;
		float mYaw;
		float mPitch;
		float mRoll;

		static const float3 sWorldUp;
	};
}


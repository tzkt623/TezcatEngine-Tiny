#pragma once
#include "Core/Head/ConfigHead.h"
#include "Core/Head/GLMHead.h"


namespace tezcat::Tiny
{
	class BaseRenderObserver;
	struct TINY_API CullHelper
	{
		struct FrustumPlan
		{
			float3 normal = { 0.0f, 1.0f, 0.0f };
			float distance = 0.f;

			FrustumPlan(const float3& p1, const float3& inNormal)
				: normal(glm::normalize(inNormal)),
				distance(glm::dot(normal, p1))
			{

			}
		};

		struct Frustum
		{
			FrustumPlan topFace;
			FrustumPlan bottomFace;

			FrustumPlan rightFace;
			FrustumPlan leftFace;

			FrustumPlan farFace;
			FrustumPlan nearFace;

		};

		void createFrustumWithCamera(BaseRenderObserver* camera, Frustum &frustum);
	};
}

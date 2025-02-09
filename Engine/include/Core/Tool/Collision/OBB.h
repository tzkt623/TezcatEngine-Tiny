#pragma once

#include "Core/Head/ConfigHead.h"
#include "Core/Head/GLMHead.h"
#include "Core/Head/TinyCpp.h"

namespace tezcat::Tiny
{
	class TINY_API OBB
	{
	public:
		OBB();
		OBB(const float3& min, const float3& max);
		OBB(const OBB& other);

		virtual ~OBB();

		float3 getCenter();
		void getCorners(std::array<float3, 8>& array);
		bool impact(const OBB& other);
		bool contain(const float3& point);
		void combine(const OBB& other);
		void updateMinMax(const std::array<float3, 8>& array);
		void transform(const float4x4& localToWorldMatrix);
		void reset();
		bool isValid();

	private:
		float3 mMin;
		float3 mMax;
	};
}

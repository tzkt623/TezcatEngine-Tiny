#pragma once

#include "Core/Head/ConfigHead.h"
#include "Core/Head/GLMHead.h"

namespace tezcat::Tiny
{
	class TINY_API BoundingBox
	{
	public:
		BoundingBox();
		BoundingBox(const float3& min, const float3& max);

		void reset(const float3& min, const float3& max);

		bool contains(const BoundingBox& other);

		bool impact(const BoundingBox& other);

		void translate(const float3& offset)
		{
			this->min += offset;
			this->max += offset;
		}

		void setPosition(float3& position)
		{
			auto d = this->getDimensions() * 0.5f;
			this->min = position - d;
			this->max = position + d;
		}

		void collect(const BoundingBox& other);
		void collect(const float3& point);

		float3 getDimensions() { return max - min; }
		float3 getPosition() { return (this->min + this->max) * 0.5f; }


		float3 max;
		float3 min;

	};

}

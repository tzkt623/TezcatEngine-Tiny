#pragma once

#include "Core/Head/ConfigHead.h"
#include "Core/Head/GLMHead.h"

namespace tezcat::Tiny
{
	class TINY_API BoundingBox
	{
	public:
		BoundingBox();
		BoundingBox(const glm::vec3& min, const glm::vec3& max);

		void reset(const glm::vec3& min, const glm::vec3& max);

		bool contains(const BoundingBox& other);

		bool impact(const BoundingBox& other);

		void translate(const glm::vec3& offset)
		{
			this->min += offset;
			this->max += offset;
		}

		void setPosition(glm::vec3& position)
		{
			auto d = this->getDimensions() * 0.5f;
			this->min = position - d;
			this->max = position + d;
		}

		void collect(const BoundingBox& other);
		void collect(const glm::vec3& point);

		glm::vec3 getDimensions() { return max - min; }
		glm::vec3 getPosition() { return (this->min + this->max) * 0.5f; }


		glm::vec3 max;
		glm::vec3 min;

	};

}

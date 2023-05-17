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
		OBB(const glm::vec3& min, const glm::vec3& max);
		OBB(const OBB& other);

		virtual ~OBB();

		glm::vec3 getCenter();
		void getCorners(std::array<glm::vec3, 8>& array);
		bool impact(const OBB& other);
		bool contain(const glm::vec3& point);
		void combine(const OBB& other);
		void updateMinMax(const std::array<glm::vec3, 8>& array);
		void transform(const glm::mat4& localToWorldMatrix);
		void reset();
		bool isValid();

	private:
		glm::vec3 mMin;
		glm::vec3 mMax;
	};



}
